import '../styles/CoachCTA.css';
import {LuRocket} from 'react-icons/lu'

export default function CoachCTA({sidebar, active }) {
  return (
    <div className={`coach-cta-container ${sidebar ? 'sidebar' : ''} ${active ? 'active' : ''}`}>
        {
            !sidebar && (
                <div className="outline"></div>
            )
        }
        <div className="main-button">
            <LuRocket />
            <p>Start AI Coach</p>
        </div>
    </div>
  );
}