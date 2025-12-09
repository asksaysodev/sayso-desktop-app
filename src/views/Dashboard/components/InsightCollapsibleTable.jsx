import dayjs from 'dayjs';
import { Fragment, useState } from 'react';
import { LuPlus, LuMinus } from 'react-icons/lu';
import './InsightCollapsibleTable.css';

export default function InsightCollapsibleTable({ groupDate, groupInsights, openedInsights, setOpenedInsights }) {

    const isCollapsed = !openedInsights.includes(groupDate);

    const handleCollapsible = (id) => {
        setOpenedInsights((prev) => {
            if (prev.length === 0) return [id]

            if (prev.includes(id)) {
                const newIds = prev.filter(i => i !== id)
                return [...newIds]
            } else {
                return [...prev, id]
            }
        })
    }

    return (
        <div className='collapsible-wrapper'>
            <button
                className={`collapsible-group-trigger ${!isCollapsed ? 'expanded' : ''}`}
                onClick={() => handleCollapsible(groupDate)}
            >
                {dayjs(groupDate).format('ddd, MMM D')}

                {isCollapsed ? <LuPlus /> :  <LuMinus />}
            </button>

            <div className={`table-container ${!isCollapsed ? 'expanded' : 'collapsed'}`}>
                <table className='insights-table'>
                    <thead>
                        <tr className='table-header'>
                            <th>Time</th>
                            <th>Insights</th>
                            <th>Lead Type</th>
                        </tr>
                    </thead>
                    <tbody>
                        {groupInsights.map(({ id, cue, date, leadType }, index) => {
                            const isLastItem = index === groupInsights.length - 1;
                            return (
                                <Fragment key={id}>
                                    <tr key={id} className='table-row'>
                                        <td className='time-cell'>
                                            {dayjs(date).format('h:mm A')}
                                        </td>
                                        <td className='insight-cell'>
                                            {cue}
                                        </td>
                                        <td className='lead-type-cell'>
                                            {leadType}
                                        </td>
                                    </tr>
                                    {!isLastItem && (
                                        <tr className='row-separator'>
                                            <td colSpan={3}>
                                                <div className='separator-line' />
                                            </td>
                                        </tr>
                                    )}
                                </Fragment>
                            )
                        })}
                    </tbody>
                </table>
            </div>
        </div>
    )
}