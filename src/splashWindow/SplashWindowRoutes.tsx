import SaysoLoader from "@/components/SaysoLoader";
import { useAuth } from "@/context/AuthContext";
import { Navigate, Route, Routes } from "react-router-dom";
import PostAuthRedirect from "./PostAuthRedirect";
import Login from "./routes/Login";
import MFAVerify from "./routes/MFAVerify";
import Permissions from "./routes/Permissions";

export default function SplashWindowRoutes() {
    const { loading } = useAuth();

    if (loading) return <SaysoLoader />;

    return (
        <Routes>
            <Route path="/" element={<PostAuthRedirect />} />
            <Route path="/login" element={<Login />} />
            <Route path="/mfa-verify" element={<MFAVerify />} />
            <Route path="/permissions" element={<Permissions />} />
            <Route path="*" element={<Navigate to="/login" replace />} />
        </Routes>
    );
}