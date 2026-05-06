interface Props {
    children?: React.ReactNode;
}

export default function MacBookMock({ children }: Props) {
    return (
        <div className="macbook-wrapper">
            <div className="macbook-lid">
                <div className="macbook-camera" />
                <div className="macbook-screen-area">
                    {children}
                </div>
            </div>
            <div className="macbook-hinge" />
            <div className="macbook-base">
                <div className="macbook-base-indent" />
            </div>
        </div>
    );
}
