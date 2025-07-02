// @deno-types="https://deno.land/std@0.168.0/http/server.d.ts"
import { serve } from 'https://deno.land/std@0.168.0/http/server.ts'

declare const Deno: {
  env: {
    get(key: string): string | undefined;
  };
};

const corsHeaders = {
  'Access-Control-Allow-Origin': '*',
  'Access-Control-Allow-Headers': 'authorization, x-client-info, apikey, content-type',
  'Access-Control-Allow-Methods': 'POST, OPTIONS'
};

// Constants for validation
const MAX_FILE_SIZE = 25 * 1024 * 1024; // 25MB
const SUPPORTED_CONTENT_TYPES = [
  'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
  'application/pdf',
  'text/plain',
  'application/msword'
] as const;

type SupportedContentType = typeof SUPPORTED_CONTENT_TYPES[number];

interface FileUpload {
  url: string;
  source: 'prospect' | 'salesrep';
  name: string;
}

interface RequestBody {
  files: FileUpload[];
  assistant_id: string;
}

interface UploadedFile {
  file_id: string;
  source: 'prospect' | 'salesrep';
  name: string;
}

interface FailedFile {
  name: string;
  error: string;
  status?: number;
}

async function downloadFile(url: string, headers: Record<string, string>): Promise<{ data: ArrayBuffer; contentType: SupportedContentType }> {
  console.log('Downloading file from URL:', url);
  console.log('Using headers:', JSON.stringify(headers, null, 2));
  
  // Add timeout to fetch request
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), 30000); // 30 second timeout
  
  try {
    // For Supabase storage URLs, try without auth headers first
    const isSupabaseStorageUrl = url.includes('storage/v1/object/public');
    let response: Response;

    if (isSupabaseStorageUrl) {
      console.log('Detected Supabase storage URL, trying without auth headers first...');
      try {
        response = await fetch(url, { 
          signal: controller.signal,
          headers: {
            'Accept': 'application/octet-stream'
          }
        });
      } catch (error) {
        console.error('Failed to fetch without auth headers:', error);
        // If that fails, try with auth headers
        response = await fetch(url, { 
          headers: {
            ...headers,
            'Accept': 'application/octet-stream'
          },
          signal: controller.signal 
        });
      }
    } else {
      // For non-Supabase URLs, use provided headers
      response = await fetch(url, { 
        headers,
        signal: controller.signal 
      });
    }
    
    clearTimeout(timeout);
    
    if (!response.ok) {
      console.error('Download failed:', {
        status: response.status,
        statusText: response.statusText,
        headers: Object.fromEntries(response.headers.entries()),
        url,
        isSupabaseStorageUrl
      });

      // Special handling for common Supabase storage errors
      if (isSupabaseStorageUrl) {
        if (response.status === 404) {
          throw new Error(`File not found in Supabase storage: ${url}`);
        } else if (response.status === 403) {
          throw new Error(`Access denied to file in Supabase storage: ${url}`);
        }
      }

      const error = new Error(`Failed to download file: ${response.statusText}`);
      Object.assign(error, {
        status: response.status,
        headers: Object.fromEntries(response.headers.entries())
      });
      throw error;
    }

    // For Supabase storage URLs, we know it's a DOCX file
    let contentType = response.headers.get('content-type') as SupportedContentType;
    if (isSupabaseStorageUrl && url.toLowerCase().endsWith('.docx')) {
      contentType = 'application/vnd.openxmlformats-officedocument.wordprocessingml.document';
    } else if (!contentType) {
      contentType = 'application/vnd.openxmlformats-officedocument.wordprocessingml.document';
    }

    console.log('Download response:', {
      status: response.status,
      contentType,
      contentLength: response.headers.get('content-length'),
      headers: Object.fromEntries(response.headers.entries())
    });

    // Skip content type validation for Supabase storage URLs since we know it's a DOCX
    if (!isSupabaseStorageUrl && !SUPPORTED_CONTENT_TYPES.includes(contentType)) {
      console.error('Unsupported content type:', {
        contentType,
        supportedTypes: SUPPORTED_CONTENT_TYPES,
        url
      });
      throw new Error(`Unsupported content type: ${contentType}`);
    }

    const data = await response.arrayBuffer();
    console.log('File downloaded successfully:', {
      size: data.byteLength,
      contentType,
      url
    });
    
    // Validate file size
    if (data.byteLength > MAX_FILE_SIZE) {
      console.error('File too large:', {
        size: data.byteLength,
        maxSize: MAX_FILE_SIZE,
        url
      });
      throw new Error(`File too large: ${data.byteLength} bytes (max ${MAX_FILE_SIZE} bytes)`);
    }

    return { data, contentType };
  } catch (error) {
    clearTimeout(timeout);
    if (error.name === 'AbortError') {
      console.error('Download timed out:', {
        url,
        timeout: '30 seconds'
      });
      throw new Error('File download timed out after 30 seconds');
    }
    console.error('Download error:', {
      name: error.name,
      message: error.message,
      status: error.status,
      headers: error.headers,
      url
    });
    throw error; // Preserve original error
  }
}

async function uploadToOpenAI(apiKey: string, fileData: ArrayBuffer, fileName: string, contentType: string): Promise<string> {
  console.log(`Uploading file to OpenAI: ${fileName} (${contentType}, ${fileData.byteLength} bytes)`);

  const formData = new FormData();
  const blob = new Blob([fileData], { type: contentType });
  formData.append('file', blob, fileName);
  formData.append('purpose', 'assistants');

  // Add timeout to fetch request
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), 60000); // 60 second timeout

  try {
    console.log('Making OpenAI API request...', {
      fileName,
      contentType,
      size: fileData.byteLength,
      purpose: 'assistants'
    });

    const response = await fetch('https://api.openai.com/v1/files', {
      method: 'POST',
      headers: {
        'Authorization': `Bearer ${apiKey}`
      },
      body: formData,
      signal: controller.signal
    });

    clearTimeout(timeout);
    const responseText = await response.text();
    
    console.log('OpenAI API Response:', {
      status: response.status,
      statusText: response.statusText,
      headers: Object.fromEntries(response.headers.entries()),
      body: responseText.slice(0, 1000) // Log first 1000 chars to avoid huge logs
    });
    
    if (!response.ok) {
      console.error('OpenAI API Error:', {
        status: response.status,
        statusText: response.statusText,
        response: responseText,
        fileName,
        contentType
      });
      const error = new Error(`OpenAI API Error: ${response.statusText}`);
      Object.assign(error, {
        status: response.status,
        response: responseText
      });
      throw error;
    }

    try {
      const data = JSON.parse(responseText);
      console.log('Successfully uploaded file to OpenAI:', {
        fileId: data.id,
        fileName,
        status: 'success'
      });
      return data.id;
    } catch (parseError) {
      console.error('Failed to parse OpenAI response:', {
        error: parseError,
        responseText,
        fileName
      });
      throw new Error(`Invalid JSON response from OpenAI: ${responseText}`);
    }
  } catch (error) {
    clearTimeout(timeout);
    if (error.name === 'AbortError') {
      console.error('OpenAI upload timed out:', {
        fileName,
        timeout: '60 seconds'
      });
      throw new Error('OpenAI upload timed out after 60 seconds');
    }
    console.error('OpenAI upload error:', {
      name: error.name,
      message: error.message,
      status: error.status,
      response: error.response,
      fileName
    });
    throw error; // Preserve original error
  }
}

async function createThread(apiKey: string): Promise<string> {
  console.log('Creating new OpenAI thread...');
  
  // Add timeout to fetch request
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), 30000); // 30 second timeout

  try {
    const response = await fetch('https://api.openai.com/v1/threads', {
      method: 'POST',
      headers: {
        'Authorization': `Bearer ${apiKey}`,
        'Content-Type': 'application/json'
      },
      signal: controller.signal
    });

    clearTimeout(timeout);
    const responseText = await response.text();
    
    if (!response.ok) {
      const error = new Error(`Failed to create thread: ${response.statusText}`);
      Object.assign(error, {
        status: response.status,
        response: responseText
      });
      throw error;
    }

    try {
      const data = JSON.parse(responseText);
      return data.id;
    } catch (parseError) {
      throw new Error(`Invalid JSON response from thread creation: ${responseText}`);
    }
  } catch (error) {
    clearTimeout(timeout);
    if (error.name === 'AbortError') {
      throw new Error('Thread creation timed out after 30 seconds');
    }
    throw error; // Preserve original error
  }
}

serve(async (req: Request) => {
  // Handle CORS preflight
  if (req.method === 'OPTIONS') {
    return new Response('ok', { headers: corsHeaders });
  }

  try {
    const { files, assistant_id } = await req.json() as RequestBody;
    
    // Log received data
    console.log('Received request:', {
      assistant_id,
      filesCount: files?.length,
      files: files.map(f => ({ name: f.name, source: f.source, url: f.url }))
    });

    // Validate input
    if (!files?.length) {
      return new Response(
        JSON.stringify({ error: 'No files provided' }),
        { headers: { ...corsHeaders, 'Content-Type': 'application/json' }, status: 400 }
      );
    }

    if (!assistant_id) {
      return new Response(
        JSON.stringify({ error: 'No assistant_id provided' }),
        { headers: { ...corsHeaders, 'Content-Type': 'application/json' }, status: 400 }
      );
    }

    const OPENAI_API_KEY = Deno.env.get('OPENAI_API_KEY');
    const SUPABASE_SERVICE_ROLE_KEY = Deno.env.get('SUPABASE_SERVICE_ROLE_KEY');

    if (!OPENAI_API_KEY || !SUPABASE_SERVICE_ROLE_KEY) {
      console.error('Missing environment variables:', {
        hasOpenAIKey: !!OPENAI_API_KEY,
        hasSupabaseKey: !!SUPABASE_SERVICE_ROLE_KEY
      });
      return new Response(
        JSON.stringify({ error: 'Missing required environment variables' }),
        { headers: { ...corsHeaders, 'Content-Type': 'application/json' }, status: 500 }
      );
    }

    // Create OpenAI thread first
    let threadId: string;
    try {
      threadId = await createThread(OPENAI_API_KEY);
      console.log('Created OpenAI thread:', threadId);
    } catch (error) {
      console.error('Thread creation failed:', error);
      return new Response(
        JSON.stringify({ 
          error: 'Failed to create OpenAI thread',
          details: error.message,
          status: error.status
        }),
        { headers: { ...corsHeaders, 'Content-Type': 'application/json' }, status: 500 }
      );
    }

    // Process files sequentially
    const uploadedFiles: UploadedFile[] = [];
    const failedFiles: FailedFile[] = [];
    const headers = {
      'Authorization': `Bearer ${SUPABASE_SERVICE_ROLE_KEY}`,
      'apikey': SUPABASE_SERVICE_ROLE_KEY
    };

    for (const file of files) {
      try {
        console.log(`Processing ${file.name}...`);
        
        // Download file
        const { data: fileData, contentType } = await downloadFile(file.url, headers);
        console.log(`Downloaded ${file.name}, size: ${fileData.byteLength} bytes, type: ${contentType}`);
        
        // Upload to OpenAI
        const fileId = await uploadToOpenAI(OPENAI_API_KEY, fileData, file.name, contentType);
        console.log(`Uploaded ${file.name} to OpenAI, file ID: ${fileId}`);
        
        uploadedFiles.push({
          file_id: fileId,
          source: file.source,
          name: file.name
        } as UploadedFile);
      } catch (error) {
        console.error(`Error processing ${file.name}:`, error);
        failedFiles.push({
          name: file.name,
          error: error.message,
          status: error.status
        } as FailedFile);
        // Continue processing other files
      }
    }

    // If no files were processed successfully, return error
    if (uploadedFiles.length === 0) {
      return new Response(
        JSON.stringify({ 
          error: 'Failed to process any files',
          failed_files: failedFiles
        }),
        { headers: { ...corsHeaders, 'Content-Type': 'application/json' }, status: 500 }
      );
    }

    // Return success response with any failed files
    const response = {
      thread_id: threadId,
      files: uploadedFiles,
      failed_files: failedFiles.length > 0 ? failedFiles : undefined
    };
    
    console.log('Final response:', response);

    return new Response(
      JSON.stringify(response),
      { headers: { ...corsHeaders, 'Content-Type': 'application/json' }, status: 200 }
    );
  } catch (error) {
    console.error('Error in edge function:', error);
    return new Response(
      JSON.stringify({
        error: 'Internal server error',
        details: error.message,
        status: error.status
      }),
      { headers: { ...corsHeaders, 'Content-Type': 'application/json' }, status: 500 }
    );
  }
}); 