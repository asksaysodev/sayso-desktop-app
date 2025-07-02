import { createClient } from 'https://esm.sh/@supabase/supabase-js@2';
// NOTE: Ensure you have 'openai' added to your import_map.json or use a direct URL
// Example for import map: "openai": "npm:openai@^4.0" 
// Or use direct URL: import OpenAI from 'https://esm.sh/openai@^4.0'; 
import OpenAI from 'npm:openai'; // Assumes import map is configured
import { corsHeaders } from '../_shared/cors.ts'; // Import shared CORS headers

// Placeholder for the system prompt - replace with content from prompts/coach.txt
const coachSystemPrompt = `
You are Sayso Live Sales-Coach... 
(TODO: Load actual prompt from file/config)
Your goal is to provide concise, actionable advice to a salesperson during a live call 
based on the transcript snippet, prospect context, and relevant documents.
Keep responses under 50 tokens. Focus on the immediate next step or question.
`.trim();

// Placeholder for embedding function - implement based on chosen embedding method
async function embed(text: string): Promise<number[]> {
  console.warn('Embed function not implemented yet!');
  // TODO: Implement actual embedding generation (e.g., call OpenAI embedding API)
  // const ai = new OpenAI({ apiKey: Deno.env.get('DENO_OPENAI_KEY') });
  // const embedding = await ai.embeddings.create({ model: 'text-embedding-ada-002', input: text });
  // return embedding.data[0].embedding;
  return Array(1536).fill(0); // Return dummy vector for now
}

console.log('Coach function initializing...');

Deno.serve(async (req: Request) => {
  // Handle CORS preflight requests
  if (req.method === 'OPTIONS') {
    return new Response('ok', { headers: corsHeaders }); // Use shared headers
  }

  try {
    const { mode, snippet, prospect_ctx, emails } = await req.json();
    console.log(`Function called with mode: ${mode}`);

    const openAIApiKey = Deno.env.get('DENO_OPENAI_KEY');
    const supabaseUrl = Deno.env.get('SUPABASE_URL'); // Make sure this is set in secrets
    const supabaseAnonKey = Deno.env.get('SUPABASE_ANON_KEY'); // Make sure this is set in secrets

    if (!openAIApiKey || !supabaseUrl || !supabaseAnonKey) {
      throw new Error('Missing environment variables (OpenAI Key or Supabase URL/Key)');
    }
    
    const ai = new OpenAI({ apiKey: openAIApiKey });
    // Remove the global header forwarding assuming function doesn't strictly need user context
    const sb = createClient(supabaseUrl, supabaseAnonKey);

    let responseData: any;
    let status: number = 200;

    if (mode === 'bootstrap') {
      console.log('Bootstrap mode - emails:', emails);
      // TODO: Implement logic to fetch initial prospect_ctx based on emails
      // This might involve querying your 'prospects' or 'contacts' table
      // and potentially associated 'account_files' or 'prospect_files'.
      // Example (replace with your actual logic):
      /*
      const { data: prospectData, error: prospectError } = await sb
        .from('prospects')
        .select('*, context_summary') // Assuming a context summary field
        .in('email', emails) 
        .limit(1)
        .single();

      if (prospectError) throw prospectError;
      const prospectCtx = prospectData?.context_summary || 'No specific context found.'; 
      */
      const fetchedProspectCtx = `Context for ${emails?.join(', ')} (Bootstrap TODO)`; // Placeholder
      responseData = { prospect_ctx: fetchedProspectCtx };
    }
    else if (mode === 'nudge') {
      console.log('Nudge mode - snippet length:', snippet?.length);
      if (!snippet || !prospect_ctx) {
        throw new Error('Missing snippet or prospect_ctx for nudge mode');
      }

      // 1. Embed the snippet (using placeholder)
      const snippetEmbedding = await embed(snippet);

      // 2. Find relevant documents using pgvector RPC call
      console.log('Calling match_docs RPC...');
      const { data: passages, error: rpcError } = await sb.rpc('match_docs', {
        q: snippetEmbedding, 
        k: 3, // Fetch top 3 matches
        tag: ['product', 'faq', 'call_good'] // Filter by tags
      });

      if (rpcError) {
        console.error('RPC Error:', rpcError);
        throw new Error(`Failed to fetch relevant documents: ${rpcError.message}`);
      }
      console.log('Found passages:', passages?.length);

      const docsContent = passages && passages.length > 0
        ? passages.map((p: { text: string }) => p.text).join('\n---\n')
        : 'No relevant documents found.';

      // 3. Construct the prompt for OpenAI
      const prompt = [
        { role: 'system' as const, content: coachSystemPrompt },
        { role: 'system' as const, content: `Prospect & Call Context:\n${prospect_ctx}` },
        { role: 'system' as const, content: `Relevant Documentation Snippets:\n${docsContent}` },
        { role: 'user' as const, content: `Live Transcript Snippet:\n${snippet}\n\nProvide a concise coaching nudge (max 50 tokens):` }
      ];

      // 4. Call OpenAI Chat Completions
      console.log('Calling OpenAI Chat Completions...');
      const chat = await ai.chat.completions.create({
        model: 'gpt-4o-mini', // Use the specified model
        messages: prompt,
        temperature: 0.3,
        max_tokens: 60 // Max tokens for the response
      });

      const nudge = chat.choices[0]?.message?.content?.trim() || null;
      console.log('Received nudge:', nudge);
      responseData = { nudge };
    }
    else {
      responseData = { error: 'Invalid mode specified' };
      status = 400;
    }

    // Return success response with CORS headers
    return new Response(JSON.stringify(responseData), {
      headers: { ...corsHeaders, 'Content-Type': 'application/json' }, // Include CORS
      status: status,
    });

  } catch (error) {
    console.error('Error in Edge Function:', error);
    // Return error response with CORS headers
    return new Response(JSON.stringify({ error: error.message }), {
      headers: { ...corsHeaders, 'Content-Type': 'application/json' }, // Include CORS
      status: 500, // Internal Server Error
    });
  }
});

console.log('Coach function request handler set up.'); 