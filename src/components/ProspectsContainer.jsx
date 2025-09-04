import { useState, useEffect } from 'react';

import CTABar from './CTABar';
import ProspectCard from './ProspectCard';
import ProspectListItem from './ProspectListItem';
import ProspectDetail from './ProspectDetail';

import {SlMagnifier} from 'react-icons/sl';
import {GoPlus} from 'react-icons/go';  
import {PiSquaresFourLight, PiListBullets } from 'react-icons/pi';
import { LuBuilding, LuPaperclip} from 'react-icons/lu'
import { useProspectsContext } from '../context/ProspectsContext';

import '../styles/ProspectsContainer.css';

export default function ProspectsContainer() {

    //STATE
    const [view, setView] = useState('cards');
    const [selectedProspect, setSelectedProspect] = useState(null);

    //HOOKS
    const { prospects, fetchProspects } = useProspectsContext();

    //FUNCTIONS
    const handleSelectProspect = (prospect) => {
        setSelectedProspect(prospect);
    }

    return (
        <div className='prospects-container-main'>
            {selectedProspect && (
                <ProspectDetail prospect={selectedProspect} setSelectedProspect={setSelectedProspect} fetchProspects={fetchProspects}/>
            )}
            <div className='prospects-container-header'>
                <h1>My Prospects</h1>
            </div>
            <CTABar active={false} />
            {
                !prospects.length > 0 ? (
                    <div className='no-prospects-found-container'>
                        <p>No prospects found. Click the button below to add your first prospect:</p>
                        <div className='add-prospect-button'>
                            <GoPlus />
                            <p>Add Prospect</p>
                        </div>
                    </div>

                ) : (
                    <div className='prospects-container-body'>
                        <div className='prospects-toolbar-container'>
                            <div className='prospect-searchbar-container'>
                                <SlMagnifier />
                                <input type="text" placeholder='Search Prospects' />
                            </div>
                            
                            <div className='full-h flex-row'>
                                <div className='prospect-view-toggle' data-view={view}>
                                <div className={`prospect-view-toggle-button ${view === 'cards' ? 'active' : ''}`} onClick={() => setView('cards')}>
                                        <PiSquaresFourLight />
                                        <p>Cards</p>
                                </div>
                                <div className={`prospect-view-toggle-button ${view === 'list' ? 'active' : ''}`} onClick={() => setView('list')}>
                                        <PiListBullets />
                                        <p>List</p>
                                </div>
                                </div>
                                <div className='add-prospect-button'>
                                    <GoPlus />
                                    <p>Add Prospect</p>
                                </div>
                                
                            </div>
                        </div>
                        <div className='prospects-list-container'>
                            {view === 'cards' && (
                                <div className='prospect-cards-container'>
                                    {prospects.map((prospect) => (
                                        <ProspectCard handleSelectProspect={handleSelectProspect} prospect={prospect} key={prospect.id} />
                                    ))}
                                </div>
                            )}
                            {view === 'list' && (
                                <div className='prospect-listview-container'>
                                    <div className='prospect-list-header'>
                                        <div className='prospect-list-name prospect-header-item'>
                                            <h4>Prospect Name</h4>
                                        </div>
                                        <div className='prospect-list-company prospect-header-item'>
                                            <LuBuilding />
                                            <h4>Company</h4>
                                        </div>
                                        <div className='prospect-list-files prospect-header-item'>
                                            <LuPaperclip />
                                            <h4>Related Files</h4>
                                        </div>
                                        <div className='prospect-list-action prospect-header-item'>
                                            <h4>Actions</h4>
                                        </div>
                                    </div>
                                    <div className='prospect-list-items-container'>
                                        {
                                            prospects.map((prospect) => (
                                                <ProspectListItem handleSelectProspect={handleSelectProspect} prospect={prospect} key={prospect.id} />
                                            ))
                                        }
                                    </div>
                                </div>
                            )}
                        </div>
                    </div>
                )
            }
        </div>
    )
}