import { LuLoader } from 'react-icons/lu';
import '../styles/Buttons.css';

const LoginPrimaryBtn = ({ text, onClick, isDisabled = false, isLoading = false, isDelete = false }) => {
  return (
    <button 
      className={`login-primary-button ${isLoading ? 'loading' : '' } ${isDelete ? 'delete' : ''}`} 
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

export default LoginPrimaryBtn; 