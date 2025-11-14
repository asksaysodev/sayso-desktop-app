import { useAuth } from '../context/AuthContext';
import SaysoModal from './SaysoModal';

export default function PermissionsModalContainer() {
    const { showPermissionsModal, setShowPermissionsModal, requestAllPermissions } = useAuth();
    if (!showPermissionsModal) return null;
    return (
        <SaysoModal
            title={"We need some permissions to work"}
            text={"Sayso needs to access your audio, we will only use it when you launch a call"}
            isDelete={false}
            onDeny={() => setShowPermissionsModal(false)}
            onConfirm={() => requestAllPermissions()}
            primaryText={"Allow"}
            secondaryText={"Cancel"}
        />
    );
}