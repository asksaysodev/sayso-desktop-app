import React from "react";

interface SettingsContentLayoutProps {
    title: string;
    children: React.ReactNode;
}

function SettingsContentLayout({ title, children }: SettingsContentLayoutProps) {
    return (
        <div className="settings-content-layout">
            <h1 className="settings-content-title">{title}</h1>
            <div className="settings-content-body">
                {children}
            </div>
        </div>
    );
}

export default SettingsContentLayout;
