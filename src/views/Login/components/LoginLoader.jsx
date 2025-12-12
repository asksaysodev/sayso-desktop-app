import { Loader } from "lucide-react";

export default function LoginLoader({ isLoggingIn }) {
    return (
        <div className="loginContainer">
            <div className="loginForm">
                <Loader message={isLoggingIn ? "Signing in..." : "Creating account..."} />
            </div>
      </div>
    )
}