import { LuUpload } from 'react-icons/lu';
import '../styles/NewFileInput.css';

export default function NewFileInput() {
    return (
        <div className="new-file-input-container">
            <LuUpload />
            <p><span>Click to upload</span> or drag and drop</p>
            <p>.docx, .pdf, .txt, .doc files (max 300mb)</p>
        </div>
    )
}