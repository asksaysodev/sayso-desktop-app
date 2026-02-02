import { useMutation, useQueryClient } from "@tanstack/react-query";
import { updateInsight } from "../services/updateInsight";
import { InsightRating } from "../types";
import * as Sentry from '@sentry/electron/renderer';
import { useToast } from "@/context/ToastContext";
import { GetInsightsResponse } from "@/types/coach";

export default function useUpdateInsights() {
    const {showToast} = useToast();
    const queryClient = useQueryClient();

    const { mutate: updateInsightRating, isPending: isUpdatingInsightRating } = useMutation({
        mutationFn: ({ insightId, rating }: { insightId: string, rating: InsightRating }) => updateInsight(insightId, rating),
        onMutate: async ({ insightId, rating }) => {
            await queryClient.cancelQueries({ queryKey: ['dashboard-insights'] });

            const previousData = queryClient.getQueriesData<GetInsightsResponse>({
                queryKey: ['dashboard-insights']
            });

            queryClient.setQueriesData<GetInsightsResponse>(
                { queryKey: ['dashboard-insights'] },
                (old) => {
                    if (!old) return old;

                    return {
                        ...old,
                        insights: old.insights.map(group => ({
                            ...group,
                            insights: group.insights.map(insight =>
                                insight.id === insightId
                                    ? { ...insight, rating }
                                    : insight
                            )
                        }))
                    };
                }
            );

            return { previousData };
        },
        onError: (error, variables, context) => {
            if (context?.previousData) {
                context.previousData.forEach(([queryKey, data]) => {
                    queryClient.setQueryData(queryKey, data);
                });
            }

            showToast('error', 'Error updating insight rating');
            console.error('Error updating insight rating:', error);
            Sentry.captureException(error);
        },
    });

    return {
        updateInsightRating,
        isUpdatingInsightRating,
    }
}