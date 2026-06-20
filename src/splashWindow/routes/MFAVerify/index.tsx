import { useEffect, useRef, useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '@/context/AuthContext';
import LoginLayout from '@/components/layouts/LoginLayout';
import LoginBtn from '@/components/LoginBtn';
import './styles.css';

const MFAVerify = () => {
    const navigate = useNavigate();
    const { verifyMFA, handleSignOut } = useAuth();

    const [digits, setDigits] = useState<string[]>(Array(6).fill(''));
    const [error, setError] = useState<string | null>(null);
    const [isVerifying, setIsVerifying] = useState(false);
    const inputRefs = useRef<(HTMLInputElement | null)[]>([]);
    // Set synchronously to guard against double-submit even before isVerifying re-renders.
    const verifyingRef = useRef(false);

    const code = digits.join('');

    const handleVerify = async () => {
        if (code.length !== 6 || verifyingRef.current) return;
        verifyingRef.current = true;
        setIsVerifying(true);
        setError(null);
        const result = await verifyMFA(code);
        if (result.success) {
            // Let PostAuthRedirect decide based on the permissions flag (skips /permissions if complete).
            navigate('/', { replace: true });
        } else {
            setError(result.error?.message || 'Invalid code. Please try again.');
            setDigits(Array(6).fill(''));
            inputRefs.current[0]?.focus();
            verifyingRef.current = false;
            setIsVerifying(false);
        }
    };

    // Auto-submit when all 6 digits filled
    useEffect(() => {
        if (code.length === 6) handleVerify();
    }, [code]);

    const handleChange = (index: number, value: string) => {
        if (!/^\d*$/.test(value)) return;
        const digit = value.slice(-1);
        const newDigits = [...digits];
        newDigits[index] = digit;
        setDigits(newDigits);
        setError(null);
        if (digit && index < 5) inputRefs.current[index + 1]?.focus();
    };

    const handleKeyDown = (index: number, e: React.KeyboardEvent<HTMLInputElement>) => {
        if (e.key === 'Backspace') {
            if (digits[index]) {
                const newDigits = [...digits];
                newDigits[index] = '';
                setDigits(newDigits);
            } else if (index > 0) {
                const newDigits = [...digits];
                newDigits[index - 1] = '';
                setDigits(newDigits);
                inputRefs.current[index - 1]?.focus();
            }
        } else if (e.key === 'ArrowLeft' && index > 0) {
            inputRefs.current[index - 1]?.focus();
        } else if (e.key === 'ArrowRight' && index < 5) {
            inputRefs.current[index + 1]?.focus();
        }
    };

    const handlePaste = (e: React.ClipboardEvent) => {
        e.preventDefault();
        const pasted = e.clipboardData.getData('text').replace(/\D/g, '').slice(0, 6);
        const newDigits = [...digits];
        pasted.split('').forEach((char, i) => { newDigits[i] = char; });
        setDigits(newDigits);
        inputRefs.current[Math.min(pasted.length, 5)]?.focus();
    };

    const handleBackToLogin = async () => {
        await handleSignOut();
        navigate('/login', { replace: true });
    };

    return (
        <LoginLayout hideLogo error={error}>
            <h1 className="permissions-title">One More Step</h1>
            <p className="permissions-subtitle">In order to protect your account, please enter the 6-digit code from your Authenticator App</p>
            <div className="mfa-otp-wrapper">
                <div className="mfa-otp-inputs">
                    {digits.map((digit, i) => (
                        <input
                            key={i}
                            ref={el => { inputRefs.current[i] = el; }}
                            type="text"
                            inputMode="numeric"
                            maxLength={1}
                            value={digit}
                            onChange={e => handleChange(i, e.target.value)}
                            onKeyDown={e => handleKeyDown(i, e)}
                            onPaste={handlePaste}
                            onFocus={e => e.target.select()}
                            className={`mfa-otp-input${digit ? ' mfa-otp-input--filled' : ''}`}
                            disabled={isVerifying}
                            autoFocus={i === 0}
                        />
                    ))}
                </div>
                <LoginBtn
                    text={isVerifying ? 'Verifying…' : 'Verify'}
                    onClick={handleVerify}
                    isLoading={isVerifying}
                    isDisabled={code.length !== 6 || isVerifying}
                />
            </div>
            <p className="mfa-layout-toggle-text" onClick={handleBackToLogin}>Back to Log In</p>
        </LoginLayout>
    );
};

export default MFAVerify;
