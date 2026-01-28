import { AdminStore } from "@/types/store/adminStore";
import { LeadType } from "@/types/coach";
import { Tool } from "@/views/Admin/types";
import { create } from "zustand";

export const useAdminStore = create<AdminStore>((set, get) => ({
    selectedTool: 'cue-signals',
    leadType: 'buyer',
    
    setSelectedTool: (selectedTool: Tool) => set({ selectedTool }),
    setLeadType: (leadType: LeadType) => set({ leadType }),
}));