import '../styles/SaysoModal.css';

export default function SaysoModal({ title, text, isDelete, onDeny, onConfirm, isModalLoading, primaryText, secondaryText }) {
    return (
        <div className="sayso-modal-view-container">
            <div className="sayso-modal-container">
                <h2>{title}</h2>
                <p>{text}</p>
                <div className={`sayso-modal-actions ${isDelete ? 'delete' : ''}`}>
                    {
                        isDelete ? (
                            <>  
                                <button onClick={onDeny} className="sayso-modal-action-secondary">{secondaryText}</button>
                                <button onClick={onConfirm} className="sayso-modal-action-primary">{primaryText}</button>
                            </>
                        ) : (
                            <>
                                <button onClick={onDeny} className="sayso-modal-action-secondary">{secondaryText}</button>
                                <button onClick={onConfirm} className="sayso-modal-action-primary">{primaryText}</button>
                            </>
                        )
                    }
                </div>
            </div>
        </div>
    );
}