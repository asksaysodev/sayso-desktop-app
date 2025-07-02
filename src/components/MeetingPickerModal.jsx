import React, { useEffect, useState } from 'react';
import '../styles/MeetingPickerModal.css';
import { useZoom } from '../hooks/useZoom';
import { useAuth } from '../context/AuthContext';
import { useTimes } from '../hooks/useTimes';
import { useProspects } from '../hooks/useProspects';
import Loader from './Loader';

const MeetingPickerModal = ({ isOpen, onClose, prospectId, handleSaveProspectMeeting }) => {

    const [meetings, setMeetings] = useState(null);

    const { getZoomMeetings } = useZoom();
    const { globalUser } = useAuth();
    const { formatTime } = useTimes();

    useEffect(() => {
        console.log('fetching meetings');
        const fetchMeetings = async () => {
            try {
                const response = await getZoomMeetings(globalUser.id);
                console.log('Zoom meetings response:', response);
                setMeetings(response.meetings);
            } catch (error) {
                console.error('Error fetching meetings:', error);
                setMeetings([]);
            }
        }
        fetchMeetings();
    }, []);

    useEffect(() => {
        console.log('meetings:', meetings);
    }, [meetings]);

    const handleMeetingClick = async (meeting) => {

        try {
            await handleSaveProspectMeeting(meeting);
            onClose();
        } catch (error) {
            console.error('Error saving prospect meeting:', error);
        }
    }

    if (!isOpen) return null;

    return (
        <div className="modal-overlay" onClick={onClose}>
            <div className="modal-content" onClick={e => e.stopPropagation()}>
                {
                    !meetings ? (
                        <>
                            <Loader  message="Loading Meetings..."/>
                        </>
                    ) : (
                        <>
                            {
                                meetings.length > 0 ? (
                                    <>
                                        <h2 style={{ marginBottom: '30px' }}>Select a Meeting for this Prospect</h2>
                                        <ul className="meeting-list">
                                            {
                                                meetings.map((meeting, idx) => (
                                                    <li className="meeting-item" key={idx} onClick={() => handleMeetingClick(meeting)}>
                                                        <div className="meeting-item-header">
                                                            <h3>{meeting.topic}</h3>
                                                            <p>{formatTime(meeting.start_time)}</p>
                                                        </div>
                                                    </li>
                                                ))
                                            }
                                        </ul>
                                    </>
                                ) : (
                                    <>
                                        <h2>No Meetings Found</h2>
                                    </>
                                )
                            }
                        </>
                    )
                }
            </div>
        </div>
    );
};

export default MeetingPickerModal;
