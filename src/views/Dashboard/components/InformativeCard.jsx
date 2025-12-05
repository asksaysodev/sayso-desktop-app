export default function InformativeCard({ title, icon, description, children }) {
    return (
        <div className="informative-card-container">
            <div className="informative-card-header">
                {icon}
                <p>{title}</p>
            </div>

            {children && children}

            <p className="informative-card-description">{description}</p>
        </div>
    )
}