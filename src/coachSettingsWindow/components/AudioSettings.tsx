import { useEffect, useRef, useState } from "react";
import { LuChevronDown } from "react-icons/lu";
import * as Sentry from "@sentry/electron/renderer";
import SettingsContentLayout from "./SettingsContentLayout";

const BAR_COUNT = 20;

interface AudioDevice {
    deviceId: string;
    label: string;
}

export default function AudioSettings() {
    const [devices, setDevices] = useState<AudioDevice[]>([]);
    const [selectedDevice, setSelectedDevice] = useState<AudioDevice | null>(null);
    const [streamError, setStreamError] = useState<string | null>(null);
    const [dropdownOpen, setDropdownOpen] = useState(false);
    const dropdownRef = useRef<HTMLDivElement>(null);
    const barsRef = useRef<HTMLDivElement>(null);
    const rafRef = useRef<number>(0);
    const streamRef = useRef<MediaStream | null>(null);
    const audioCtxRef = useRef<AudioContext | null>(null);

    useEffect(() => {
        async function getDevices() {
            try {
                await navigator.mediaDevices.getUserMedia({ audio: true, video: false });
                const all = await navigator.mediaDevices.enumerateDevices();
                const inputs: AudioDevice[] = all
                    .filter(d => d.kind === 'audioinput' && d.label)
                    .map(d => ({ deviceId: d.deviceId, label: d.label }));
                setDevices(inputs);
                const def = inputs.find(d => d.deviceId === 'default') ?? inputs[0] ?? null;
                setSelectedDevice(def);
            } catch {
                setDevices([]);
            }
        }
        getDevices();
    }, []);

    useEffect(() => {
        if (!selectedDevice) return;

        let analyser: AnalyserNode;
        let dataArray: Uint8Array;
        let cancelled = false;

        async function startStream() {
            try {
                setStreamError(null);
                const constraint = selectedDevice!.deviceId === 'default'
                    ? { audio: true }
                    : { audio: { deviceId: { exact: selectedDevice!.deviceId } } };

                const stream = await navigator.mediaDevices.getUserMedia(constraint);

                if (cancelled) {
                    stream.getTracks().forEach(t => t.stop());
                    return;
                }

                streamRef.current = stream;
                const audioCtx = new AudioContext();
                audioCtxRef.current = audioCtx;
                analyser = audioCtx.createAnalyser();
                analyser.fftSize = 512;
                analyser.smoothingTimeConstant = 0.75;
                dataArray = new Uint8Array(analyser.frequencyBinCount);
                audioCtx.createMediaStreamSource(stream).connect(analyser);
                draw();
            } catch (err) {
                Sentry.captureException(err);
                if (!cancelled) {
                    const message = err instanceof DOMException ? err.message : 'Could not access microphone.';
                    setStreamError(message);
                }
            }
        }

        function draw() {
            if (cancelled) return;
            
            rafRef.current = requestAnimationFrame(draw);
            analyser.getByteTimeDomainData(dataArray);

            let sum = 0;
            for (let i = 0; i < dataArray.length; i++) {
                const v = (dataArray[i] / 128.0) - 1;
                sum += v * v;
            }
            const activeBars = Math.min(BAR_COUNT, Math.round(Math.sqrt(sum / dataArray.length) * BAR_COUNT * 5));

            if (barsRef.current) {
                const bars = barsRef.current.children;
                for (let i = 0; i < bars.length; i++) {
                    bars[i].className = `audio-level-bar${i < activeBars ? ' active' : ''}`;
                }
            }
        }

        startStream();

        return () => {
            cancelled = true;
            cancelAnimationFrame(rafRef.current);
            streamRef.current?.getTracks().forEach(t => t.stop());
            audioCtxRef.current?.close();
        };
    }, [selectedDevice]);

    useEffect(() => {
        function handleClickOutside(e: MouseEvent) {
            if (dropdownRef.current && !dropdownRef.current.contains(e.target as Node)) {
                setDropdownOpen(false);
            }
        }
        document.addEventListener('mousedown', handleClickOutside);
        return () => document.removeEventListener('mousedown', handleClickOutside);
    }, []);

    return (
        <SettingsContentLayout title="Audio">
            <div id="audio-input-device" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Input Device</span>
                    <p className="cue-setting-description">The microphone used during your call.</p>
                </div>
                <div className="cue-setting-right">
                    {devices.length === 0 ? (
                        <span className="audio-device-name">—</span>
                    ) : (
                        <div className="settings-dropdown" ref={dropdownRef}>
                            <button
                                className="settings-dropdown-trigger"
                                onClick={() => setDropdownOpen(v => !v)}
                            >
                                <span>{selectedDevice?.label ?? '—'}</span>
                                <LuChevronDown className={`settings-dropdown-chevron ${dropdownOpen ? 'open' : ''}`} />
                            </button>
                            {dropdownOpen && (
                                <div className="settings-dropdown-menu">
                                    {devices.map(device => (
                                        <button
                                            key={device.deviceId}
                                            className={`settings-dropdown-item ${selectedDevice?.deviceId === device.deviceId ? 'active' : ''}`}
                                            onClick={() => {
                                                setSelectedDevice(device);
                                                setDropdownOpen(false);
                                            }}
                                        >
                                            {device.label}
                                        </button>
                                    ))}
                                </div>
                            )}
                        </div>
                    )}
                </div>
            </div>
            <div id="audio-input-level" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Input Level</span>
                    <p className="cue-setting-description">Live input level from your microphone.</p>
                </div>
                <div className="cue-setting-right">
                    <div className="audio-level-meter" ref={barsRef}>
                        {Array.from({ length: BAR_COUNT }).map((_, i) => (
                            <span key={i} className="audio-level-bar" />
                        ))}
                    </div>
                    {streamError && <p className="audio-stream-error">{streamError}</p>}
                </div>
            </div>
        </SettingsContentLayout>
    );
}
