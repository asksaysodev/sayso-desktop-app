import axios from 'axios';

export const getDynamicContext = async (data) => {
  try {
    const response = await axios.post(`${import.meta.env.VITE_BACKEND_BASE_URL}/sales-coach/dynamic-context`, data);
    return response.data;
  } catch (error) {
    console.error('Error getting dynamic context:', error);
    throw error;
  }
};

// Helper function to format dynamicContext into readable text with enhanced instructions
const formatDynamicContext = (dynamicContext) => {
  if (!dynamicContext) return 'No context available';
  
  let contextText = 'CONTEXT FOR SALES COACHING:\n\n';
  
  // Handle prospect context
  if (dynamicContext.prospectContext && dynamicContext.prospectContext.length > 0) {
    contextText += '📋 PROSPECT BACKGROUND & NEEDS:\n';
    contextText += 'Use this information to understand the prospect\'s situation, company, and potential pain points:\n';
    dynamicContext.prospectContext.forEach((item, index) => {
      const relevance = (parseFloat(item.similarity) * 100).toFixed(1);
      contextText += `${index + 1}. [Relevance: ${relevance}%] ${item.text}\n`;
    });
    contextText += '\n';
  }
  
  // Handle account context - ENHANCED to highlight product info
  if (dynamicContext.accountContext && dynamicContext.accountContext.length > 0) {
    contextText += '💼 PRODUCT KNOWLEDGE & FEATURES:\n';
    contextText += '**USE THIS INFORMATION TO SUGGEST SPECIFIC PRODUCT FEATURES, PRICING, OR CAPABILITIES:**\n';
    dynamicContext.accountContext.forEach((item, index) => {
      const relevance = (parseFloat(item.similarity) * 100).toFixed(1);
      contextText += `${index + 1}. [Relevance: ${relevance}%] ${item.text}\n`;
    });
  }
  
  if (!dynamicContext.prospectContext?.length && !dynamicContext.accountContext?.length) {
    contextText += 'No relevant context found for this conversation.\n';
    contextText += 'Focus on general sales best practices and active listening techniques.';
  }
  
  // Add product-focused coaching instructions
  const totalContext = (dynamicContext.prospectContext?.length || 0) + (dynamicContext.accountContext?.length || 0);
  if (totalContext > 0) {
    contextText += '\n\n🎯 PRODUCT-FOCUSED COACHING:\n';
    contextText += '- When prospect asks about features, suggest SPECIFIC product tiers or capabilities\n';
    contextText += '- When discussing pricing, reference exact amounts from the context\n';
    contextText += '- When covering coverage, mention specific pests, limits, or guarantees\n';
    contextText += '- Prioritize product-specific guidance over general sales advice\n';
  }
  
  return contextText;
};

export const trackSignals = async (currentConversation) => {
  if(!currentConversation) {
    throw new Error('Missing required parameters');
  }

  try {

    const systemMessage = {
      role: "system",
      content: `You are detecting sales signals from a live sales call transcript. Analyze the dialogue and return whether the following signals are present. For each detected signal, return the exact quote that triggered it. Interpret meaning flexibly — do not rely on exact phrasing. Use semantic understanding.

      SIGNAL TO DETECT:
      1. Pain Points
        Definition: Signs of frustration, inefficiency, unmet needs, or manual/broken processes.
        Examples:
        - “We are struggling with…”
        - “It is frustrating when…”
        - “This takes too long…”
        - “We do it manually…”
        - “It is kind of a mess”

      2. Economic Impact
        Definition: Mentions of cost, budget limits, time savings, ROI, or financial constraints.
        Examples:
        - “We are spending too much…”
        - “It is costing us…”
        - “We could save time if…”

      3. Decision Makers
        Definition: Mentions of who controls buying decisions, authority limits, or stakeholders.
        Examples:
        - “I will need to run it by…”
        - “Leadership decides that…”
        - “We have a team for that…”
        - “I am not the final decision-maker”

        4. Buying Process Objections
        Definition: Expressions of hesitation, friction, uncertainty, or resistance.
        Examples:
        - “Now is not the best time…”
        - “We have tried this before…”
        - “We already have a solution…”
        - “It sounds good, but…”

      Current conversation to analyze:
      ${currentConversation}
      
      You must return a JSON object with all 4 signals. Here is the required format:

      {
        "pain_points": {
          "detected": false,
          "quote": null
        },
        "economic_impact": {
          "detected": false,
          "quote": null
        },
        "decision_makers": {
          "detected": false,
          "quote": null
        },
        "objections": {
          "detected": false,
          "quote": null
        }
      }

      CRITICAL: 
      - Return ONLY a JSON object (starting with { and ending with })
      - Include ALL 4 signal types: pain_points, economic_impact, decision_makers, objections
      - Set "detected" to true only if the signal is clearly present
      - Include the exact quote in "quote" field if detected, otherwise null
      - Do NOT return an array, return a single object with all signals

      - Only return true if the signal is clearly implied. Use your best reasoning. Always include the quote if detected.    
      - Only return the quote that triggered the signal. Do not return all quotes.
      - Property named "detected" should only be true or false (boolean).
      - Property named "quote" should only be the quote that triggered the signal. Do not return all quotes. Only return a quote if "detected" is true.
      - If a signal has been detected, do not return false for that signal, only change the "detected" property to true when the signal is detected.
      -`
    };
    
    const messages = [systemMessage];
    
    const response = await axios.post(`${import.meta.env.VITE_BACKEND_BASE_URL}/sales-coach/chat-completion`, {
      messages: messages
    });

    const chatCompletionResponse = response.data.response;


    // Add better error handling for JSON parsing
    let parsedResponse;
    try {
      parsedResponse = JSON.parse(chatCompletionResponse);
    } catch (parseError) {
      console.error('❌ Failed to parse trackSignals response as JSON:', {
        response: chatCompletionResponse,
        error: parseError.message
      });
      // Return a safe default response instead of throwing
      return [
        { signal: "pain_points", detected: false, quote: null },
        { signal: "economic_impact", detected: false, quote: null },
        { signal: "decision_makers", detected: false, quote: null },
        { signal: "objections", detected: false, quote: null }
      ];
    }

    // Validate the response structure - now expecting an object, not an array
    if (!parsedResponse || typeof parsedResponse !== 'object' || Array.isArray(parsedResponse)) {
      console.error('❌ trackSignals response is not a valid object:', parsedResponse);
      return [
        { signal: "pain_points", detected: false, quote: null },
        { signal: "economic_impact", detected: false, quote: null },
        { signal: "decision_makers", detected: false, quote: null },
        { signal: "objections", detected: false, quote: null }
      ];
    }

    // Validate and sanitize each signal from the object format
    const expectedSignals = ["pain_points", "economic_impact", "decision_makers", "objections"];
    const validatedResponse = expectedSignals.map(expectedSignal => {
      const signalData = parsedResponse[expectedSignal];
      
      if (!signalData) {
        console.warn(`⚠️ Missing signal: ${expectedSignal}, using default`);
        return { signal: expectedSignal, detected: false, quote: null };
      }

      // Ensure detected is a boolean
      const detected = Boolean(signalData.detected);
      
      // Ensure quote is a string or null
      let quote = null;
      if (detected && signalData.quote) {
        quote = String(signalData.quote).trim();
        if (quote === '') quote = null;
      }

      return {
        signal: expectedSignal,
        detected: detected,
        quote: quote
      };
    });

    return validatedResponse;

  } catch (error) {
    console.error('❌ Error in trackSignals:', error);
    // Return a safe default response instead of throwing
    return [
      { signal: "pain_points", detected: false, quote: null },
      { signal: "economic_impact", detected: false, quote: null },
      { signal: "decision_makers", detected: false, quote: null },
      { signal: "objections", detected: false, quote: null }
    ];
  }
};

export const runChatCompletion = async (currentConversation, dynamicContext, previousInsights, signals, callProgress) => {

  if(!currentConversation || !dynamicContext || !previousInsights) {
    throw new Error('Missing required parameters');
  }

  try {   

    // Parse previous insights - they might be objects with Message property
    const previousInsightsFormatted = previousInsights.map((insight, index) => {
      // Handle both string and object formats
      const insightText = typeof insight === 'string' ? insight : insight.Message || insight.message || JSON.stringify(insight);
      return `${index + 1}. ${insightText}`;
    }).join('\n');

    // Format dynamicContext into readable text
    const contextText = formatDynamicContext(dynamicContext);

    const systemMessage = {
      role: "system",
      content: `You are a world-class sales coach assisting a rep during a live sales conversation. Your goal is to provide real-time, actionable guidance based on the conversation so far, relevant context, and the current state of the call.
    
      ${contextText}
      
      CALL CONTEXT:
      The call is currently at this stage: "${callProgress}".
      Only offer coaching appropriate for this stage.
      For example, avoid objections or pricing advice too early.
      
      SALES SIGNAL STATUS:
      Use this to determine what areas still need to be addressed. Only offer coaching insights for signals that are still missing.
      
      Signals detected so far:
      ${signals.map(s => `- ${s.signal}: ${s.detected ? `✅ "${s.quote}"` : '❌ Not yet detected'}`).join('\n')}
      
      COACHING INSTRUCTIONS:
      - ONLY analyze the **most recent exchange** between the sales rep (USER) and the prospect (PROSPECT).
      - Wait until **at least one back-and-forth** has occurred before offering any insight.
      - **DO NOT repeat insights already delivered**.
      - **PRIORITIZE PRODUCT-SPECIFIC GUIDANCE**: Use the sales rep context to suggest specific features, pricing tiers, or product capabilities.
      - **BE SPECIFIC WITH PRODUCT DETAILS**: When relevant, mention exact product names, coverage levels, pricing, or features from the context.
      - Suggest **exact product tiers or features** to mention (e.g., "Premium plan covers 15+ pests").
      - Reference **specific pricing or coverage details** from the context
      - **Do NOT** suggest generic sales strategies unless tied to specific product features
      - Focus only on **signals that are still missing** from the list above.
      - Suggest helpful follow-up questions or nudges to surface the missing signals, when appropriate.
      - **If the call is too early to surface certain insights (e.g., pricing/objection), hold off** until a later stage.
      
      COACHING FORMAT:
      - Be concise: MAX 150 characters.
      - If no actionable insight applies, say: "No insight needed at this time."
      
      Previously Given Insights:
      ${previousInsightsFormatted}
      
      Current Conversation to Analyze:
      ${currentConversation}
      
      Respond in this exact JSON format:
      {
        "Insight": "yes|no",
        "Message": "If Insight is 'yes', provide specific, actionable feedback in under 150 characters. If Insight is 'no', write 'No insight needed at this time.'"
      }`  
    };
    
    
    const messages = [systemMessage];
    
    const response = await axios.post(`${import.meta.env.VITE_BACKEND_BASE_URL}/sales-coach/chat-completion`, {
      messages: messages
    });

    const chatCompletionResponse = response.data.response;

    // Add better error handling for JSON parsing
    let parsedResponse;
    try {
      parsedResponse = JSON.parse(chatCompletionResponse);
    } catch (parseError) {
      console.error('❌ Failed to parse runChatCompletion response as JSON:', {
        response: chatCompletionResponse,
        error: parseError.message
      });
      // Return a safe default response instead of throwing
      return {
        Insight: "no",
        Message: "No insight needed at this time."
      };
    }

    return parsedResponse;

  } catch (error) {
    console.error('❌ Error in runChatCompletion:', error);
    // Return a safe default response instead of throwing
    return {
      Insight: "no",
      Message: "No insight needed at this time."
    };
  }
};
