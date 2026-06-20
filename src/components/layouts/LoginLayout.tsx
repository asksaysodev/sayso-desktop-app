import './styles/loginLayout.css';
import saysoLogoHorizontal from '/assets/logo-neg-horizontal.png';

interface Props {
    children: React.ReactNode;
    title?: string;
    description?: string;
    error?: string | null;
    notice?: string | null;
    hideLogo?: boolean;
}

export default function LoginLayout({ children, title, description, error, notice, hideLogo }: Props) {
    return (
        <div className="loginLayoutContainer">
            <div className="splash-drag-bar"></div>
            <div className="loginLayoutForm">
                {!hideLogo && <img src={saysoLogoHorizontal} className="loginLayoutLogo" alt="Sayso" />}
                {description && <p className="loginLayoutDescription">{description}</p>}
                {notice && <div className="noticeMessage">{notice}</div>}
                <div className={`errorMessage${error ? '' : ' errorMessage--hidden'}`}>{error}</div>
                {children}
            </div>
        </div>
    );
}
