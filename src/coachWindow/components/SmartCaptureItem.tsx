import { useState } from 'react';

import { IoChevronDown , IoChevronUp } from 'react-icons/io5';
import { FaCheck } from 'react-icons/fa6';

interface SignalData {
    detected: boolean;
    quote: string | null;
}

interface Props {
    signal: SignalData | null;
    title: string;
}

export default function SmartCaptureItem({signal, title}: Props) {

    //STATE
    const [isOpen, setIsOpen] = useState(false);

    //FUNCTIONS
    const trimPhrase = (phrase: string): string => {
        if(!phrase || phrase === '') {
            return '...';
        }
        return '...' + phrase.substring(0, 90) + '...';
    }

    // console.log('signal received', signal, title);
    // const fakemessage = "I'd say we're definitely considering all the options. You've made a good case and I'll take everything we just said discussed into account before making a final decision";

    
    return (
        <li className="smart-capture-item" data-open={isOpen} data-detected={signal?.detected}>
            <div className='smart-capture-item-header'>
                <div className='smart-capture-item-header-content'>
                    <span>
                        {signal?.detected && <FaCheck /> }
                    </span>
                    <h4>{title}</h4>
                </div>
                <div className='smart-capture-item-header-actions'>
                    {
                        signal?.detected && (
                            <button className='smart-capture-item-header-action' onClick={() => setIsOpen(!isOpen)} disabled={!signal?.detected}>
                                {isOpen ? <IoChevronUp /> : <IoChevronDown />}
                            </button>
                        )
                    }
                </div>
            </div>
            {
                isOpen && signal?.quote && signal?.quote !== '' && (
                    <div className='smart-capture-item-content'>
                        <p>{trimPhrase(signal?.quote)}</p>
                    </div>
                )
            }
        </li>
    )
}