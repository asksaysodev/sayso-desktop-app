import type { LoginBannerVariant } from './loginHeadline';

interface Props {
    variant: LoginBannerVariant;
    children: React.ReactNode;
}

export default function LoginBanner({ variant, children }: Props) {
    return (
        <div className={`splash-banner splash-banner--${variant}`}>
            {children}
        </div>
    );
}
