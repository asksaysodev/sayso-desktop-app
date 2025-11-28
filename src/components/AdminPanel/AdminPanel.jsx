import { useState } from 'react';
import { LuLoader } from 'react-icons/lu';

import { useAdmin } from '../../hooks/useAdmin';
import { useToast } from '../../context/ToastContext';

import './AdminPanel.css';


export default function AdminPanel() {

    //HOOKS
    const { postCueSignals } = useAdmin();
    const { showToast } = useToast();

    //STATE
    const [leadType, setLeadType] = useState('buyer');
    const [signals, setSignals] = useState('');
    const [isSubmitting, setIsSubmitting] = useState(false); 

    //FUNCTIONS
    const handleSubmit = async () => {
        setIsSubmitting(true);
        try {
            if(!leadType || !signals) {
                throw new Error('Lead type and signals are required');
            }
            const signalsArray = JSON.parse(signals);

            if(!Array.isArray(signalsArray)) {
                throw new Error('Signals must be an array');
            }

            // Validate all signal objects to only include 'priority', 'id', 'description', and 'instructions' (no other properties)
            const allowedProps = ['priority', 'id', 'description', 'instructions'];
            const allSignalsSanitized = signalsArray.map((item, idx) => {
                // Check for unwanted properties
                const keys = Object.keys(item);
                if (!(
                    typeof item.priority === 'number' &&
                    typeof item.id === 'string' &&
                    typeof item.description === 'string' &&
                    typeof item.instructions === 'string'
                )) {
                    throw new Error(`Signal at index ${idx} is missing required properties or has invalid types.`);
                }
                // Ensure no extra properties
                const extraProps = keys.filter(k => !allowedProps.includes(k));
                if (extraProps.length > 0) {
                    throw new Error(`Signal at index ${idx} contains extra properties: ${extraProps.join(', ')}`);
                }
                // Return only the allowed properties (defensive; deletes extras if present)
                return {
                    priority: item.priority,
                    id: item.id,
                    description: item.description,
                    instructions: item.instructions
                };
            });
            const response = await postCueSignals(leadType, signals);
            console.log('response', response);
            setSignals('');
            showToast('success', 'Signals submitted successfully!');
        } catch (error) {
            console.error('Error submitting signals:', error);
            showToast('error', 'Failed to submit signals' + error.message);
        } finally {
            setIsSubmitting(false);
        }
    }
    return (
        <div className='admin-panel-main'>
            <div className='admin-panel-header'>
                <h2>Admin Panel</h2>
            </div>
            <div className='admin-panel-body'>
                <div className='admin-panel-selector-container'>
                    <label>Select a lead type</label>
                    <select value={leadType} onChange={(e) => setLeadType(e.target.value)}>
                        <option value="buyer">Buyer</option>
                        <option value="seller">Seller</option>
                    </select>


                </div>
                <div className='admin-panel-input-container'>
                    <label>Enter the signals</label>
                    <textarea placeholder="Enter the signals" value={signals} onChange={(e) => setSignals(e.target.value)} />
                </div>
                <div className='admin-panel-submit-button-container'><button onClick={handleSubmit} disabled={isSubmitting}>Submit</button></div>
                    {isSubmitting && <div className='admin-panel-loading-container'><LuLoader /></div>}
            </div>
        </div>
    )
}