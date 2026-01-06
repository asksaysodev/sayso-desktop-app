import { LuEye, LuEyeOff } from "react-icons/lu";

export default function EyeToggleShowPasswordButton({ showPassword, togglePasswordVisibility}) {
     return (
         <button
             type="button"
             className="passwordToggleButton"
             onClick={togglePasswordVisibility}
             aria-label={showPassword ? 'Hide password' : 'Show password'}
         >
             {showPassword ? <LuEyeOff size={18} /> : <LuEye size={18} />}
         </button>
     )
 }
