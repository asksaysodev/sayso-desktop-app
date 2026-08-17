import { useEffect, useRef, useState } from 'react';
import { CloudUpload, X } from 'lucide-react';
import { Spinner } from '@/components/ui/spinner';
import { PLAYBOOK_LIMIT, validateFile } from '../constants';

interface UploadCardProps {
    disabled: boolean;
    isUploading: boolean;
    uploadError: string | null;
    onUpload: (file: File, alias: string) => void;
    onClearError: () => void;
}

export default function UploadCard({
    disabled,
    isUploading,
    uploadError,
    onUpload,
    onClearError,
}: UploadCardProps) {
    const inputRef = useRef<HTMLInputElement | null>(null);
    const aliasInputRef = useRef<HTMLInputElement | null>(null);
    const [file, setFile] = useState<File | null>(null);
    const [alias, setAlias] = useState('');
    const [validationError, setValidationError] = useState<string | null>(null);
    const [isDragOver, setIsDragOver] = useState(false);

    const canUpload =
        !disabled &&
        !isUploading &&
        file !== null &&
        alias.trim().length > 0 &&
        validationError === null;

    const acceptFile = (incoming: File) => {
        const error = validateFile(incoming);
        if (error) {
            setValidationError(error);
            setFile(null);
            return;
        }
        setValidationError(null);
        setFile(incoming);
        onClearError();
    };

    const handleBrowse = () => {
        if (disabled) return;
        inputRef.current?.click();
    };

    const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
        const picked = e.target.files?.[0];
        if (picked) acceptFile(picked);
        e.target.value = '';
    };

    const handleDrop = (e: React.DragEvent) => {
        e.preventDefault();
        setIsDragOver(false);
        if (disabled) return;
        const dropped = e.dataTransfer.files?.[0];
        if (dropped) acceptFile(dropped);
    };

    const handleDragOver = (e: React.DragEvent) => {
        e.preventDefault();
        if (disabled) return;
        setIsDragOver(true);
    };

    const handleDragLeave = () => setIsDragOver(false);

    useEffect(() => {
        if (file) {
            aliasInputRef.current?.focus();
        }
    }, [file]);

    const handleClearFile = (e: React.MouseEvent) => {
        e.stopPropagation();
        setFile(null);
        setValidationError(null);
    };

    const handleSubmit = () => {
        if (!canUpload || !file) return;
        onUpload(file, alias.trim());
        setFile(null);
        setAlias('');
    };

    const errorMessage = validationError ?? uploadError;

    return (
        <div id="playbooks-upload" className={`playbooks-upload-card ${disabled ? 'disabled' : ''}`}>
            {file ? (
                <div className="playbooks-drop-file">
                    <span className="playbooks-drop-file-name" title={file.name}>
                        {file.name}
                    </span>
                    <button
                        type="button"
                        className="playbooks-drop-file-clear"
                        onClick={handleClearFile}
                        aria-label="Clear selected file"
                    >
                        <X size={14} />
                    </button>
                </div>
            ) : (
                <div
                    className={`playbooks-drop-zone ${isDragOver ? 'drag-over' : ''}`}
                    onClick={handleBrowse}
                    onDrop={handleDrop}
                    onDragOver={handleDragOver}
                    onDragLeave={handleDragLeave}
                    role="button"
                    tabIndex={disabled ? -1 : 0}
                >
                    <div className="playbooks-drop-icon">
                        <CloudUpload size={16} />
                    </div>
                    <div className="playbooks-drop-text">
                        <p className="playbooks-drop-primary">
                            Drop a file here, or{' '}
                            <span className="playbooks-drop-browse">browse</span>
                        </p>
                        <p className="playbooks-drop-secondary">
                            PDF, DOCX, TXT — up to 20 MB
                        </p>
                    </div>
                    <input
                        ref={inputRef}
                        type="file"
                        accept=".pdf,.docx,.txt"
                        style={{ display: 'none' }}
                        onChange={handleFileChange}
                    />
                </div>
            )}

            {file && (
                <>
                    <div className="playbooks-upload-divider" />

                    <div className="playbooks-upload-row">
                        <div className="playbooks-alias-field">
                            {/* <label className="playbooks-alias-label" htmlFor="playbook-alias-input">
                                ALIAS
                            </label> */}
                            <input
                                ref={aliasInputRef}
                                id="playbook-alias-input"
                                className="playbooks-alias-input"
                                type="text"
                                placeholder="Enter a name for this Script"
                                value={alias}
                                onChange={(e) => setAlias(e.target.value)}
                                disabled={disabled || isUploading}
                            />
                        </div>
                        <button
                            type="button"
                            className="playbooks-upload-btn"
                            disabled={!canUpload}
                            onClick={handleSubmit}
                        >
                            {isUploading ? <Spinner width={14} height={14} /> : 'Upload'}
                        </button>
                    </div>
                </>
            )}

            {disabled && (
                <p className="playbooks-upload-message">
                    Limit reached — you can have up to {PLAYBOOK_LIMIT} custom playbooks.
                </p>
            )}
            {errorMessage && !disabled && (
                <p className="playbooks-upload-error">{errorMessage}</p>
            )}
        </div>
    );
}
