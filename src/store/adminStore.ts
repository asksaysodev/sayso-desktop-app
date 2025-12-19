import { Tool } from "@/views/Admin/types";
import { create } from "zustand";

type LeadType = 'buyer' | 'seller';

interface AdminStore {
    leadType: LeadType;
    setLeadType: (leadType: LeadType) => void;
    selectedTool: Tool;
    setSelectedTool: (selectedTool: Tool) => void;
}

export const useAdminStore = create<AdminStore>((set, get) => ({
    selectedTool: 'cue-signals',
    leadType: 'buyer',
    
    setSelectedTool: (selectedTool: Tool) => set({ selectedTool }),
    setLeadType: (leadType: LeadType) => set({ leadType }),
}));