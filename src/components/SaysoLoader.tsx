import '../styles/SaysoLoader.css';
import saysoLogo from '/assets/sayso.svg';

export default function SaysoLoader() {
	return (
		<div className="sayso-loader-container">
			<div className="sayso-loader-wrap">
				<div className="sayso-loader-orbit">
					<span className="sayso-loader-dot" />
				</div>
				<img src={saysoLogo} className="sayso-loader-logo" alt="Loading Sayso" />
			</div>
		</div>
	);
}
