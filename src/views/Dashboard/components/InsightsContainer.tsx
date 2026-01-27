import { useMemo, useState, useRef, useEffect } from 'react';
import { LuUsers, LuSearch, LuCommand, LuHistory } from 'react-icons/lu';
import SaysoPopover from '@/components/SaysoPopover';

import LeadTypeFilterSelector from './LeadTypeFilterSelector';
import InsightsCalendarPopover, { INITIAL_DATE_RANGE } from './InsightsCalendarPopover';
import ActiveFilters from "./ActiveFilters";

import {
  InputGroup,
  InputGroupAddon,
  InputGroupInput,
} from "@/components/ui/input-group";
import dayjs from "dayjs";
import InsightCollapsibleTable from './InsightCollapsibleTable';
import InsightsListSkeleton from './InsightsListSkeleton';
import { DateRange } from 'react-day-picker';

import isBetween from 'dayjs/plugin/isBetween';
import isSameOrAfter from 'dayjs/plugin/isSameOrAfter';
import getInsights from '../services/getInsights';
import { useQuery } from '@tanstack/react-query';
import SaysoInputGroup from '@/components/forms/SaysoInputGroup';
import type { LeadTypeFilter, Insight, InsightGroup } from '@/types/coach';

dayjs.extend(isBetween);
dayjs.extend(isSameOrAfter);

export default function InsightsContainer() {
    const [selectedLeadTypeFilter, setSelectedLeadTypeFilter] = useState<LeadTypeFilter>('all');
    const [searchInsightInputValue, setSearchInsightInputValue] = useState('');
    const [dateRangeFilter, setDateRangeFilter] = useState<DateRange>(INITIAL_DATE_RANGE);
    const [openedInsights, setOpenedInsights] = useState<string[]>([]);
    const [isScrolled, setIsScrolled] = useState(false);
    const listContainerRef = useRef<HTMLDivElement>(null);
    const [page, setPage] = useState(0);
    const [allInsights, setAllInsights] = useState<InsightGroup[]>([]);

    const { data: insightsData, isLoading: isLoadingInsights, error: errorInsights, isFetching, isRefetching: isRefetchingInsights } = useQuery({
        queryKey: ['dashboard-insights', page],
        queryFn: () => getInsights(page)
    });
    
    const { insights = [], hasNextPage = false} = insightsData || {};

    useEffect(() => {
        if (insights.length > 0) {
            setAllInsights(prev => {
                if (page === 0) {
                    return insights;
                }
                
                const existingDates = new Map(prev.map(item => [item.date, item]));
                
                insights.forEach(newGroup => {
                    const existing = existingDates.get(newGroup.date);
                    if (existing) {
                        const existingIds = new Set(existing.insights.map(i => i.id));
                        const newInsights = newGroup.insights.filter(i => !existingIds.has(i.id));
                        existing.insights.push(...newInsights);
                    } else {
                        existingDates.set(newGroup.date, newGroup);
                    }
                });
                
                return Array.from(existingDates.values()).sort((a, b) => b.date.localeCompare(a.date));
            });
        }
    }, [insights, page]);

    /**
     * Checks if a date falls within the specified date range (inclusive)
     * If 'to' is not provided, it only checks if the date matches the 'from' date
     */
    function checkIfDateIsAvailable(date: string | Date, dateRange: DateRange): boolean {
        if (!dateRange.to) {
            return dayjs(date).isSameOrAfter(dayjs(dateRange.from), 'day');
        }

        return dayjs(date).isBetween(dateRange.from, dateRange.to, 'day', '[]');
    }

    const filteredInsights = useMemo(() => {
        const searchQuery = searchInsightInputValue.trim().toLowerCase();
        
        const filtered = allInsights
            .map(({ date, insights: groupInsights }) => {
                let filteredGroupInsights = groupInsights;

                if (selectedLeadTypeFilter !== 'all') {
                    filteredGroupInsights = filteredGroupInsights.filter(insight => 
                        insight.lead_type === selectedLeadTypeFilter
                    );
                }

                if (searchQuery) {
                    filteredGroupInsights = filteredGroupInsights.filter(insight => 
                        insight.message.toLowerCase().includes(searchQuery)
                    );
                }

                if (dateRangeFilter && dateRangeFilter.from) {
                    filteredGroupInsights = filteredGroupInsights.filter(insight => 
                        checkIfDateIsAvailable(insight.timestamp, dateRangeFilter)
                    );
                }

                return { date, insights: filteredGroupInsights };
            })
            .filter(group => group.insights.length > 0);

        return filtered;
    }, [selectedLeadTypeFilter, allInsights, searchInsightInputValue, dateRangeFilter])

    useEffect(() => {
        const listContainer = listContainerRef.current;
        if (!listContainer) return;

        const handleScroll = () => {
            setIsScrolled(listContainer.scrollTop > 0);

            const { scrollTop, scrollHeight, clientHeight } = listContainer;
            const isNearBottom = scrollHeight - scrollTop - clientHeight < 100;

            if (isNearBottom && hasNextPage && !isFetching) {
                setPage(prev => prev + 1);
            }
        };

        listContainer.addEventListener('scroll', handleScroll);
        
        return () => {
            listContainer.removeEventListener('scroll', handleScroll);
        };
    }, [hasNextPage, isFetching]);

    return (
        <div className='insights-container'>
            <div className={`insights-header ${isScrolled ? 'scrolled' : ''}`}>
                <p>Insights</p>

                <div className='insights-header-right-content'>
                   <div>
                        <SaysoInputGroup
                            className='h-[40px]'
                            placeholder='Search Insight...'
                            value={searchInsightInputValue}
                            onChange={(e) => setSearchInsightInputValue(e.target.value)}
                            icon={<LuSearch />}
                            size={20}
                        />
                   </div>


                    <SaysoPopover 
                        popoverContent={<LeadTypeFilterSelector selectedLeadTypeFilter={selectedLeadTypeFilter} setSelectedLeadTypeFilter={setSelectedLeadTypeFilter} />}
                    >
                        <LuUsers /> Lead Type
                    </SaysoPopover>

                    <InsightsCalendarPopover applyDateRangeFilter={setDateRangeFilter} />
                </div>
            </div>

            <ActiveFilters 
                selectedLeadTypeFilter={selectedLeadTypeFilter}
                searchInsightInputValue={searchInsightInputValue}
                dateRangeFilter={dateRangeFilter}
                onClearLeadType={() => setSelectedLeadTypeFilter('all')}
                onClearSearch={() => setSearchInsightInputValue('')}
                onClearDateRange={() => setDateRangeFilter(INITIAL_DATE_RANGE)}
            />

            <div className='insights-list-container' ref={listContainerRef}>
                {errorInsights ? (
                    <div className='empty-insights-container'>
                        <p className='empty-insights-text'>
                            Unable to load insights. Please try again later.
                        </p>
                    </div>
                ) : (isLoadingInsights && filteredInsights.length === 0) || isRefetchingInsights ? (
                    <InsightsListSkeleton />
                ) : filteredInsights.length > 0 ? (
                    <>
                        {filteredInsights.map(({ date, insights: groupInsights }) => {
                            return (
                                <InsightCollapsibleTable 
                                    key={date}
                                    groupDate={date} 
                                    groupInsights={groupInsights}
                                    openedInsights={openedInsights} 
                                    setOpenedInsights={setOpenedInsights}
                                />
                            )
                        })}
                        {isFetching && page > 0 && (
                            <div className='empty-insights-container'>
                                <p className='empty-insights-text'>Loading more insights...</p>
                            </div>
                        )}
                    </>
                ) : (
                    <div className='empty-insights-container'>
                        <p className='empty-insights-text'>
                            {allInsights.length === 0 ? 'No Insights yet.' : 'No results found...'}
                        </p>
                    </div>
                )}
            </div>
        </div>
    );
}
