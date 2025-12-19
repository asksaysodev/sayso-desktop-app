import './layouts.css';

export default function ViewLayout({ title, children }) {
    return (
        <div className="view-layout-container">
            <div className='view-container-header'>
                <h1>{title}</h1>
            </div>
            
            {children}
        </div>
    )
}