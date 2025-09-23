import { LuLoader } from 'react-icons/lu';
import '../styles/Buttons.css';

const LoginBtn = ({ text, onClick, isDisabled = false, isLoading = false, isDelete = false, isSecondary = false }) => {
  return (
    <button 
      className={`login-primary-button ${isLoading ? 'loading' : '' } ${isDelete ? 'delete' : ''} ${isSecondary ? 'secondary' : ''}`} 
      onClick={onClick} 
      disabled={isDisabled}
      data-loading={isLoading}>
        <p>{text}</p>
        {isLoading && (
          <div className="button-loader">
            <LuLoader />
          </div>
        )}
    </button>
  );
};

export default LoginBtn; 