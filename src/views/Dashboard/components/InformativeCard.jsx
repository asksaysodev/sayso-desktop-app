export default function InformativeCard({ title, icon, description, children, rightContent }) {
    return (
        <div className="informative-card-container">
            <div className="informative-card-header">
                <div className="informative-card-header-title">
                    {icon}
                    <p>{title}</p>
                </div>

                {rightContent}
            </div>

            <div className="informative-card-content-container">
                {children && children}

                <p className="informative-card-description">{description}</p>
            </div>
        </div>
    )
}