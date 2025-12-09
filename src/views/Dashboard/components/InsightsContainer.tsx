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
import { DateRange } from 'react-day-picker';

import isBetween from 'dayjs/plugin/isBetween';
import isSameOrAfter from 'dayjs/plugin/isSameOrAfter';

dayjs.extend(isBetween);
dayjs.extend(isSameOrAfter);

type LeadType = 'all' | 'buyer' | 'seller';
const INSIGHTS = [
    {
        id: '001',
        leadType: 'Seller',
        cue: 'Smelly cat, Smelly cat, what are they feeding you?',
        date: '2025-12-05T14:30:00'
    },
    {
        id: '0',
        leadType: 'Seller',
        cue: 'El mas grande lejos',
        date: '2025-12-05T14:30:00'
    },
    {
        id: '1',
        leadType: 'Seller',
        cue: 'Coach flagged strong buying signals',
        date: '2025-12-05T14:30:00'
    },
    {
        id: '2',
        leadType: 'Buyer',
        cue: 'Asked about mortgage pre-approval process',
        date: '2025-12-05T10:15:00'
    },
    {
        id: '3',
        leadType: 'Seller',
        cue: 'Mentioned urgent timeline for selling',
        date: '2025-12-05T16:45:00'
    },
    {
        id: '4',
        leadType: 'Buyer',
        cue: 'Requested second showing this week',
        date: '2025-12-04T11:20:00'
    },
    {
        id: '5',
        leadType: 'Seller',
        cue: 'Discussed recent comparable sales in area',
        date: '2025-12-04T09:30:00'
    },
    {
        id: '6',
        leadType: 'Buyer',
        cue: 'Coach detected high engagement on property photos',
        date: '2025-12-08T13:00:00'
    },
    {
        id: '7',
        leadType: 'Seller',
        cue: 'Asked about staging and renovation ROI',
        date: '2025-12-08T15:15:00'
    },
    {
        id: '8',
        leadType: 'Buyer',
        cue: 'Mentioned specific neighborhood preferences',
        date: '2025-11-25T10:45:00'
    },
    {
        id: '9',
        leadType: 'Seller',
        cue: 'Coach flagged pricing concerns in conversation',
        date: '2025-11-22T14:00:00'
    },
    {
        id: '10',
        leadType: 'Buyer',
        cue: 'Brought up closing timeline expectations',
        date: '2025-11-20T12:30:00'
    },
    {
        id: '11',
        leadType: 'Seller',
        cue: 'Expressed interest in quick cash offers',
        date: '2025-11-18T17:00:00'
    },
    {
        id: '12',
        leadType: 'Buyer',
        cue: 'Coach detected urgency in finding property',
        date: '2025-11-15T09:45:00'
    },
    {
        id: '13',
        leadType: 'Seller',
        cue: 'Inquired about best time to list property',
        date: '2025-11-14T11:00:00'
    },
    {
        id: '14',
        leadType: 'Buyer',
        cue: 'Discussed school district requirements',
        date: '2025-11-13T14:20:00'
    },
    {
        id: '15',
        leadType: 'Seller',
        cue: 'Coach noted concerns about current market conditions',
        date: '2025-11-12T10:30:00'
    },
    {
        id: '16',
        leadType: 'Buyer',
        cue: 'Asked about inspection process details',
        date: '2025-11-11T15:45:00'
    },
    {
        id: '17',
        leadType: 'Seller',
        cue: 'Mentioned relocating for work soon',
        date: '2025-11-10T09:15:00'
    },
    {
        id: '18',
        leadType: 'Buyer',
        cue: 'Expressed interest in new construction options',
        date: '2025-11-08T13:30:00'
    },
    {
        id: '19',
        leadType: 'Seller',
        cue: 'Asked about open house strategies',
        date: '2025-11-07T16:00:00'
    },
    {
        id: '20',
        leadType: 'Buyer',
        cue: 'Coach flagged budget flexibility signals',
        date: '2025-11-06T10:00:00'
    },
    {
        id: '21',
        leadType: 'Seller',
        cue: 'Discussed property disclosure requirements',
        date: '2025-11-05T14:45:00'
    },
    {
        id: '22',
        leadType: 'Buyer',
        cue: 'Asked about down payment assistance programs',
        date: '2025-11-04T11:30:00'
    },
    {
        id: '23',
        leadType: 'Seller',
        cue: 'Mentioned wanting to upgrade to larger home',
        date: '2025-11-03T15:00:00'
    },
    {
        id: '24',
        leadType: 'Buyer',
        cue: 'Coach detected strong emotional connection to property',
        date: '2025-11-01T12:00:00'
    },
    {
        id: '25',
        leadType: 'Seller',
        cue: 'Inquired about marketing plan details',
        date: '2025-12-06T10:30:00'
    },
    {
        id: '26',
        leadType: 'Buyer',
        cue: 'Asked about HOA fees and restrictions',
        date: '2025-12-07T14:15:00'
    },
    {
        id: '27',
        leadType: 'Seller',
        cue: 'Discussed potential repairs before listing',
        date: '2025-12-01T09:00:00'
    },
    {
        id: '28',
        leadType: 'Buyer',
        cue: 'Coach noted interest in investment properties',
        date: '2025-11-29T16:30:00'
    },
    {
        id: '29',
        leadType: 'Seller',
        cue: 'Asked about exclusive listing agreements',
        date: '2025-11-26T13:45:00'
    },
    {
        id: '30',
        leadType: 'Buyer',
        cue: 'Mentioned pre-qualification letter ready',
        date: '2025-11-24T11:15:00'
    },
]

export default function InsightsContainer() {
    const [selectedLeadTypeFilter, setSelectedLeadTypeFilter] = useState<LeadType>('all');
    const [searchInsightInputValue, setSearchInsightInputValue] = useState('');
    const [dateRangeFilter, setDateRangeFilter] = useState<DateRange>(INITIAL_DATE_RANGE);
    const [openedInsights, setOpenedInsights] = useState([]);
    const [isScrolled, setIsScrolled] = useState(false);
    const listContainerRef = useRef<HTMLDivElement>(null);

    const groupedInsightsByDate = useMemo(() => {
        return INSIGHTS.reduce((acc, insight) => {
            const dateKey = dayjs(insight.date).format('YYYY-MM-DD');
            const existing = acc.get(dateKey) || [];
            acc.set(dateKey, [...existing, insight]);
            return acc;
        }, new Map<string, typeof INSIGHTS>());
    }, []);

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

    /**
     * Sorts insights by date in descending order (newest first)
     * Takes array of [dateKey, insights[]] entries and returns sorted array
     */
    function sortByDateDesc(entries: [string, typeof INSIGHTS][]): [string, typeof INSIGHTS][] {
        return entries.sort((a, b) => {
            return b[0].localeCompare(a[0]);
        });
    }

    const filteredInsights = useMemo(() => {
        const filteredMap = new Map<string, typeof INSIGHTS>();
        const searchQuery = searchInsightInputValue.trim().toLowerCase();
        
        groupedInsightsByDate.forEach((insights, dateKey) => {
            let filtered = insights;

            if (selectedLeadTypeFilter !== 'all') {
                filtered = filtered.filter(insight => 
                    insight.leadType.trim().toLowerCase() === selectedLeadTypeFilter.trim().toLowerCase()
                );
            }

            if (searchQuery) {
                filtered = filtered.filter(insight => 
                    insight.cue.toLowerCase().includes(searchQuery)
                );
            }

            if (dateRangeFilter && dateRangeFilter.from) {
                filtered = filtered.filter(insight => checkIfDateIsAvailable(insight.date, dateRangeFilter))
            }

            if (filtered.length > 0) {
                filteredMap.set(dateKey, filtered);
            }
        });

        const sortedEntries = sortByDateDesc(Array.from(filteredMap));
        return sortedEntries;
    }, [selectedLeadTypeFilter, groupedInsightsByDate, searchInsightInputValue, dateRangeFilter])

    useEffect(() => {
        const listContainer = listContainerRef.current;
        if (!listContainer) return;

        const handleScroll = () => {
            setIsScrolled(listContainer.scrollTop > 0);
        };

        listContainer.addEventListener('scroll', handleScroll);
        
        return () => {
            listContainer.removeEventListener('scroll', handleScroll);
        };
    }, []);

    return (
        <div className='insights-container'>
            <div className={`insights-header ${isScrolled ? 'scrolled' : ''}`}>
                <p>Insights</p>

                <div className='insights-header-right-content'>
                   <div>
                        <InputGroup className='h-[40px]'>
                            <InputGroupInput 
                                placeholder='Search Insight...'
                                value={searchInsightInputValue}
                                onChange={(e) => setSearchInsightInputValue(e.target.value)}
                            />
                            <InputGroupAddon>
                                <LuSearch />
                            </InputGroupAddon>
                            <InputGroupAddon align='inline-end'>
                                <LuCommand />
                                <span>+ K</span>
                            </InputGroupAddon>
                        </InputGroup>
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
                {filteredInsights.length > 0 
                    ? filteredInsights.map(([ date, insights ]) => {
                        return (
                            <InsightCollapsibleTable 
                                    key={date}
                                    groupDate={date} 
                                    groupInsights={insights}
                                    openedInsights={openedInsights} 
                                    setOpenedInsights={setOpenedInsights}
                                />
                            )
                        }) 
                    : <div className='empty-insights-container'>
                        {/* <LuSearch /> */}
                        <p className='empty-insights-text'>No results found...</p>
                    </div>
                    }
            </div>
        </div>
    );
}
