import { PlaybookSuggestion } from '../helpers/searchPlaybook';
import highlightText from '../helpers/highlightText';

interface Props {
    suggestions: PlaybookSuggestion[];
    activeIndex: number;
    onSelect: (suggestion: PlaybookSuggestion) => void;
}

function matchLabel(suggestion: PlaybookSuggestion) {
    if (suggestion.matchCount === 0) return 'Name match';
    return `${suggestion.matchCount} ${suggestion.matchCount === 1 ? 'match' : 'matches'}`;
}

export default function FindSuggestions({ suggestions, activeIndex, onSelect }: Props) {
    if (suggestions.length === 0) {
        return (
            <div className="playbook-find-panel playbook-find-empty">
                <div className="playbook-find-empty-title">No results</div>
                <div className="playbook-find-empty-hint">Not in this playbook or any other.</div>
            </div>
        );
    }

    return (
        <div className="playbook-find-panel">
            <div className="playbook-find-panel-header">
                {suggestions.length} other {suggestions.length === 1 ? 'playbook' : 'playbooks'}
            </div>
            <ul className="playbook-find-suggestions" role="listbox">
                {suggestions.map((suggestion, index) => (
                    <li
                        key={suggestion.playbookId}
                        id={`playbook-suggestion-${suggestion.playbookId}`}
                        role="option"
                        aria-selected={index === activeIndex}
                        className={`playbook-find-suggestion ${index === activeIndex ? 'is-active' : ''}`}
                        onMouseDown={(e) => e.preventDefault()}
                        onClick={() => onSelect(suggestion)}
                    >
                        <div className="playbook-find-suggestion-top">
                            <span className="playbook-find-suggestion-label">
                                {highlightText(suggestion.label, suggestion.labelRanges)}
                            </span>
                            <span className="playbook-find-suggestion-count">{matchLabel(suggestion)}</span>
                        </div>
                        {suggestion.snippet && (
                            <div className="playbook-find-suggestion-snippet">
                                {highlightText(suggestion.snippet.text, [
                                    { start: suggestion.snippet.start, end: suggestion.snippet.end },
                                ])}
                            </div>
                        )}
                    </li>
                ))}
            </ul>
        </div>
    );
}
