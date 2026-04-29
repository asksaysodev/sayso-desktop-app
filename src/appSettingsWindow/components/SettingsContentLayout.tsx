import React from "react";

interface SettingsContentLayoutProps {
    title: string;
    description?: string;
    children: React.ReactNode;
}

function SettingsContentLayout({ title, children, description }: SettingsContentLayoutProps) {
    return (
        <div className="settings-content-layout">
            <h1 className="settings-content-title">{title}</h1>
            {description && <span className="settings-content-description">{description}</span>}
            <div className="settings-content-body">
                {children}
            </div>
        </div>
    );
}

export default SettingsContentLayout;
