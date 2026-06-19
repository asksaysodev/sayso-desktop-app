import './styles/loginLayout.css';

interface Props {
    children: React.ReactNode;
    title?: string;
    description?: string;
    error?: string | null;
    notice?: string | null;
}

export default function LoginLayout({ children, title, description, error, notice }: Props) {
    return (
        <div className="loginLayoutContainer">
            <div className="splash-drag-bar"></div>
            <div className="loginLayoutForm">
                <img
                    src="/assets/logo-neg-vertical.png"
                    alt="Sayso"
                    className="loginLayoutLogo"
                />
                {description && <p className="loginLayoutDescription">{description}</p>}
                {notice && <div className="noticeMessage">{notice}</div>}
                <div className={`errorMessage${error ? '' : ' errorMessage--hidden'}`}>{error}</div>
                {children}
            </div>
        </div>
    );
}
