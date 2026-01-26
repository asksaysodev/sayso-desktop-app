import { Tool } from "@/views/Admin/types";
import { LeadType } from "@/types/coach";

export interface AdminStore {
    leadType: LeadType;
    setLeadType: (leadType: LeadType) => void;
    selectedTool: Tool;
    setSelectedTool: (selectedTool: Tool) => void;
}