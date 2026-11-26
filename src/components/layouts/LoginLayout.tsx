import './styles/loginLayout.css';
import saysoLogoHorizontal from '/assets/logo-neg-horizontal.png';
import LoginBanner from './LoginBanner';
import { resolveLoginHeadline } from './loginHeadline';

interface Props {
    children: React.ReactNode;
    title?: string;
    description?: string;
    error?: string | null;
    notice?: string | null;
    hideLogo?: boolean;
}

export default function LoginLayout({ children, title, description, error, notice, hideLogo }: Props) {
    const headline = resolveLoginHeadline({ error, notice, title });

    return (
        <div className="loginLayoutContainer">
            <div className="splash-drag-bar"></div>
            <div className="loginLayoutForm">
                {!hideLogo && <img src={saysoLogoHorizontal} className="splash-layout-logo" alt="Sayso" />}
                <div className="splash-layout-headline">
                    {headline?.kind === 'banner' && <LoginBanner variant={headline.variant}>{headline.text}</LoginBanner>}
                    {headline?.kind === 'title' && <h1 className="loginLayoutTitle">{headline.text}</h1>}
                </div>
                {description && <p className="loginLayoutDescription">{description}</p>}
                {children}
            </div>
        </div>
    );
}
