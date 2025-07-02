import { serve } from 'https://deno.land/std@0.168.0/http/server.ts'

interface RequestBody {
  thread_id: string;
  assistant_id?: string;
  speaker: 'salesrep' | 'prospect';
  text: string;
  trigger_run?: boolean;
}

interface OpenAIMessage {
  id: string;
  // Add other OpenAI message properties as needed
}

interface OpenAIRun {
  id: string;
  // Add other OpenAI run properties as needed
}

serve(async (req) => {
  try {
    const body = await req.json() as RequestBody;
    const { thread_id, assistant_id, speaker, text, trigger_run = false } = body;
    const OPENAI_API_KEY = Deno.env.get('OPENAI_API_KEY');

    if (!OPENAI_API_KEY) {
      throw new Error('OPENAI_API_KEY is not set');
    }

    // Validate input
    if (!thread_id || !speaker || !text) {
      return new Response(
        JSON.stringify({ error: 'Missing thread_id, speaker, or text' }), 
        { status: 400 }
      );
    }

    if (!['salesrep', 'prospect'].includes(speaker)) {
      return new Response(
        JSON.stringify({ error: 'Speaker must be "salesrep" or "prospect"' }), 
        { status: 400 }
      );
    }

    // Step 1: Add message to thread
    const messageRes = await fetch(`https://api.openai.com/v1/threads/${thread_id}/messages`, {
      method: 'POST',
      headers: {
        Authorization: `Bearer ${OPENAI_API_KEY}`,
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        role: 'user',
        content: `${speaker}: ${text}`,
      }),
    });

    if (!messageRes.ok) {
      throw new Error(`Failed to add message: ${await messageRes.text()}`);
    }

    const message = await messageRes.json() as OpenAIMessage;

    // Optional: Step 2 - trigger a run if needed (e.g., at the end of the call)
    let run: OpenAIRun | null = null;
    if (trigger_run && assistant_id) {
      const runRes = await fetch(`https://api.openai.com/v1/threads/${thread_id}/runs`, {
        method: 'POST',
        headers: {
          Authorization: `Bearer ${OPENAI_API_KEY}`,
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ assistant_id }),
      });

      if (!runRes.ok) {
        throw new Error(`Failed to create run: ${await runRes.text()}`);
      }

      run = await runRes.json() as OpenAIRun;
    }

    return new Response(
      JSON.stringify({
        message: 'Transcript message added successfully',
        thread_id,
        message_id: message.id,
        ...(run ? { run_id: run.id } : {}),
      }), 
      { 
        status: 200,
        headers: {
          'Content-Type': 'application/json',
        },
      }
    );
  } catch (err) {
    const error = err as Error;
    return new Response(
      JSON.stringify({ 
        error: 'Unexpected error', 
        details: error.message 
      }), 
      { 
        status: 500,
        headers: {
          'Content-Type': 'application/json',
        },
      }
    );
  }
});
