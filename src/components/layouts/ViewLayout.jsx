import './layouts.css';

export default function ViewLayout({ title, children, scrollable = false }) {
    return (
        <div className={`view-layout-container ${scrollable ? 'scrollable' : ''}`}>
            <div className='view-container-header'>
                <h1>{title}</h1>
                {rightContent && rightContent}
            </div>
            
            {children}
        </div>
    )
}