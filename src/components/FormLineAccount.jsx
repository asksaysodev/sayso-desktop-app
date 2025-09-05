import { useState, useEffect } from 'react';
import { IoCheckmark, IoCloseOutline, IoPencilOutline } from 'react-icons/io5';
import { LuLoader } from 'react-icons/lu';
import { useToast } from '../context/ToastContext';

export default function FormLineAccount({ label, name, placeholder, value, onChange, onSave, onCancel }) {

    //STATE
    const [isEditing, setIsEditing] = useState(false);
    const [inputValue, setInputValue] = useState(value);
    const [isSaving, setIsSaving] = useState(false);

    //FUNCTIONS
    const handleSave = () => {
        onSave(inputValue);
    }

    const handleClose = () => {
        onCancel();
    }

    const handleChange = (e) => {
        setInputValue(e.target.value);
    }

    const handleKeyPress = (e) => {
        if(e.key === 'Enter') {
            handleSave();
        }
    }

    return (
        <label className='prospect-detail-body-information-container-label' htmlFor={name}>{label}
            <div className={`prospect-detail-body-information-input-container ${isSaving ? 'saving' : ''}`}>
                {
                    isEditing ? (
                        <>
                            <input type="text" placeholder={placeholder} id={name} name={name} value={inputValue} onChange={handleChange} disabled={isSaving} onKeyDown={handleKeyPress} />
                            <div className='prospect-form-input-icon-container'>
                                <IoCloseOutline onClick={handleClose} />
                                <IoCheckmark onClick={handleSave} />
                            </div>

                        </>
                    ) : (
                        <>
                            <p>{inputValue}</p>
                            {
                                isSaving ? (
                                    <LuLoader className='prospect-form-input-icon-container-loader' />
                                ) : (
                                    <>
                                        <IoPencilOutline onClick={() => setIsEditing(true)} />
                                    </>
                                )
                            }
                            
                        </>
                    )
                }
            </div>
        </label>
    )
}