import { LuSearch, LuCommand } from 'react-icons/lu';

export default function InsightsContainer() {
    return (
        <div className='insights-container'>
            <div className="insights-header">
                <p>Insights</p>

                <div className="insights-header-right-content">
                    <div className="search-input-container">
                        <LuSearch className="search-input-icon-left" />
                        <input type="text" placeholder="Search Project..." className="search-input" />
                        <div className="search-input-shortcut">
                            <LuCommand />
                            <span>+ K</span>
                        </div>
                    </div>
                    
                </div>
            </div>
        </div>
    );
}