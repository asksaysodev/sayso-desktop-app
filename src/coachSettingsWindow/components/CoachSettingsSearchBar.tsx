import { SetStateAction } from "react";
import { LuSearch, LuX } from "react-icons/lu";

interface Props {
    onChangeText: React.Dispatch<SetStateAction<string>>;
    value: string;
}

export default function CoachSettingsSeachBar({ onChangeText, value }: Props) {
    return (
        <div className="sidebar-search-wrapper">
            <LuSearch className="sidebar-search-icon" />
            <input
                className="sidebar-search"
                placeholder="Search"
                value={value}
                onChange={(e) => onChangeText(e.target.value)}
            />
            {value && (
                <LuX className="sidebar-search-clear" onClick={() => onChangeText('')} />
            )}
        </div>
    )
}