import { Loader2Icon } from "lucide-react"

import { cn } from "@/lib/utils"
import "./spinner.css"

interface SpinnerProps extends Omit<React.ComponentProps<"svg">, "width" | "height"> {
  size?: number
  width?: number
  height?: number
}

function Spinner({ className, size, width, height, ...props }: SpinnerProps) {
  const w = width ?? size ?? 16
  const h = height ?? size ?? w

  return (
    <Loader2Icon
      role="status"
      aria-label="Loading"
      width={w}
      height={h}
      className={cn("ui-spinner", className)}
      {...props}
    />
  )
}

export { Spinner }
