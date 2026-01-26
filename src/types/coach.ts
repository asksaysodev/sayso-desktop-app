/**
 * Sales Coach type definitions
 */

// Signal types
export type SignalType =
  | 'pain_points'
  | 'economic_impact'
  | 'decision_makers'
  | 'objections';

export interface Signal {
  signal: SignalType;
  detected: boolean;
  quote: string | null;
}

export interface SignalsMap {
  pain_points: SignalData;
  economic_impact: SignalData;
  decision_makers: SignalData;
  objections: SignalData;
}

export interface SignalData {
  detected: boolean;
  quote: string | null;
}

// Dynamic Context types
export interface DynamicContext {
  prospectContext: ContextItem[];
  accountContext: ContextItem[];
}

export interface ContextItem {
  text: string;
  similarity: string | number;
  metadata?: Record<string, unknown>;
}

// Insight types
export interface InsightResponse {
  Insight: 'yes' | 'no';
  Message: string;
}

export interface CoachInsight {
  id?: string;
  message: string;
  timestamp: number;
  type?: InsightType;
}

export type InsightType = 'suggestion' | 'warning' | 'info' | 'success';

// Call Progress types
export type CallProgress =
  | 'opening'
  | 'discovery'
  | 'presentation'
  | 'objection_handling'
  | 'closing'
  | 'follow_up';

// Coach run parameters
export interface RunCoachParams {
  conversationContext: string;
  insights: CoachInsight[];
  signals: Signal[];
  callProgress: CallProgress;
  prospectId: string;
}

export interface DynamicContextRequest {
  prospectId: string;
  accountId: string;
  conversationContext?: string;
}

// Chat completion types
export interface ChatMessage {
  role: 'system' | 'user' | 'assistant';
  content: string;
}

export interface ChatCompletionRequest {
  messages: ChatMessage[];
  model?: string;
  temperature?: number;
  max_tokens?: number;
}

export interface ChatCompletionResponse {
  response: string;
  usage?: {
    prompt_tokens: number;
    completion_tokens: number;
    total_tokens: number;
  };
}

// Conversation types
export interface ConversationTurn {
  speaker: 'USER' | 'PROSPECT';
  text: string;
  timestamp?: number;
}

export interface Conversation {
  id: string;
  turns: ConversationTurn[];
  startedAt: number;
  endedAt?: number;
}

// Coach session state
export interface CoachSessionState {
  isActive: boolean;
  callId: string | null;
  prospectId: string | null;
  callProgress: CallProgress;
  signals: Signal[];
  insights: CoachInsight[];
  conversationHistory: ConversationTurn[];
  dynamicContext: DynamicContext | null;
}

// Prospect types (related to coach functionality)
export interface Prospect {
  id: string;
  name: string;
  lastname: string;
  email?: string;
  files: File[];
  company?: string;
}
