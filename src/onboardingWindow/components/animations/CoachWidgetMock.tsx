import { FaStop } from 'react-icons/fa6';
import { GrPowerReset } from 'react-icons/gr';
import { MdDragIndicator } from 'react-icons/md';
import { LuChevronDown, LuChevronUp, LuSettings, LuX, LuSearch, LuCircleCheck, LuList } from 'react-icons/lu';

export type CoachWidgetState = 'idle' | 'active';

const SAYSO_PATH = "M38.11,47.13c-.09.28-.27.54-.48.71.08-.08.12-.18.17-.28.08-.17.17-.31.31-.43ZM58.78,42.42c0,3.24-1.73,6.06-4.34,7.62l-.24.15-.09.05L6.09,80.67l-.07.04s-.02,0-.03,0c-.17.11-.37.17-.59.17-.65,0-1.17-.52-1.17-1.17,0-.28.1-.54.27-.75,0-.03,0-.06.02-.07l33-30.94s.07-.07.09-.1,0-.01,0-.02c.22-.17.39-.43.48-.71h0c.05-.16.08-.32.08-.49,0-.9-.72-1.62-1.61-1.62H8.89c-4.9,0-8.89-3.99-8.89-8.89,0-3.38,1.88-6.32,4.66-7.83l.04-.02L49.23.2l.04-.03s.02,0,.03,0h0c.17-.1.37-.16.59-.16.64,0,1.17.52,1.17,1.17,0,.28-.1.55-.27.75,0,.03,0,.05-.02.06l-29.51,28.61s-.04.05-.05.07c-.25.19-.44.45-.53.77-.06.15-.09.32-.09.49,0,.89.72,1.61,1.61,1.61h27.69c4.91,0,8.89,3.99,8.89,8.9Z";

const DROPDOWN_OPTIONS = ['Buyer', 'Seller'];

interface Props {
    state: CoachWidgetState;
    leadType?: string | null;
    timer?: string;
    showDropdown?: boolean;
    highlightDropdownItem?: string | null;
}

export default function CoachWidgetMock({ state, leadType = null, timer = '0:00:00', showDropdown = false, highlightDropdownItem = null }: Props) {
    return (
        <div className="coach-widget-mock">
            <div className="cwm-drag-area">
                <MdDragIndicator className="cwm-drag-icon" />
                <div className="cwm-divider" />
            </div>

            {state === 'idle' && (
                <div className="cwm-idle-content">
                    <div className="cwm-lead-type-wrapper">
                        <button className={`cwm-lead-type-btn${leadType ? ' cwm-lead-type-btn--has-value' : ''}`}>
                            {leadType && (
                                <div className="cwm-avatar">{leadType[0]}</div>
                            )}
                            <span>{leadType ?? 'Select Lead Type'}</span>
                            {showDropdown ? <LuChevronUp size={13} /> : <LuChevronDown size={13} />}
                        </button>
                        {showDropdown && (
                            <div className="cwm-dropdown">
                                {DROPDOWN_OPTIONS.map((opt, i) => (
                                    <div key={opt}>
                                        {i > 0 && <div className="cwm-dropdown-separator" />}
                                        <div className={`cwm-dropdown-item${highlightDropdownItem === opt ? ' cwm-dropdown-item--highlighted' : ''}`}>
                                            {opt}
                                        </div>
                                    </div>
                                ))}
                            </div>
                        )}
                    </div>
                    {leadType && (
                        <button className="cwm-launch-btn">
                            <svg viewBox="0 0 58.78 80.89" width="12" height="12">
                                <path d={SAYSO_PATH} fill="white" />
                            </svg>
                            <span>Start</span>
                        </button>
                    )}
                </div>
            )}

            {state === 'active' && (
                <div className="cwm-active-content">
                    <button className="cwm-lead-type-btn cwm-lead-type-btn--selected">
                        <div className="cwm-avatar">{(leadType ?? 'Buyer')[0]}</div>
                        <span>{leadType ?? 'Buyer'}</span>
                    </button>
                    <div className="cwm-active-buttons">
                        <div className="cwm-search-pill">
                            <LuSearch size={12} />
                            <span>Zip Code</span>
                        </div>
                        <button className="cwm-action-btn cwm-action-btn--reset">
                            <GrPowerReset size={13} />
                        </button>
                        <button className="cwm-action-btn cwm-action-btn--stop">
                            <FaStop size={13} />
                        </button>
                        <div className="cwm-timer">{timer}</div>
                    </div>
                </div>
            )}

            <div className="cwm-right-actions">
                {state === 'idle' ? (
                    <>
                        <button className="cwm-action-btn cwm-action-btn--settings">
                            <LuSettings size={14} />
                        </button>
                        <button className="cwm-action-btn cwm-action-btn--close">
                            <LuX size={14} />
                        </button>
                    </>
                ) : (
                    <>
                        <button className="cwm-action-btn cwm-action-btn--check">
                            <LuCircleCheck size={14} />
                        </button>
                        <button className="cwm-action-btn cwm-action-btn--list">
                            <LuList size={14} />
                        </button>
                    </>
                )}
            </div>
        </div>
    );
}
