import SmartCaptureItem from './SmartCaptureItem';
import Divider from '../../components/Divider';

import '../styles/SmartCaptureBox.css';

export default function SmartCaptureBox({signals}) {
    return (
        <div className='smart-capture-container'>
            <ul>
                <SmartCaptureItem signal={signals?.pain_points} title="Pain Points"/>
                <div className='smart-capture-item-divider'></div>
                <SmartCaptureItem signal={signals?.economic_impact} title="Economic Impact" />
                <div className='smart-capture-item-divider'></div>
                <SmartCaptureItem signal={signals?.decision_makers} title="Decision Makers" />
                <div className='smart-capture-item-divider'></div>
                <SmartCaptureItem signal={signals?.objections} title="Buying Process Objections" />
            </ul>
        </div>
    )
}