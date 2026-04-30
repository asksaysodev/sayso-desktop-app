// Helper function to parse and wrap backeted text
export default function formatText(text:string) {
    if (!text) return text;
    const parts = text.split(/(\[.*?\])/g);
    return parts.map((part,index) => {
        if (part.startsWith('[') && part.endsWith(']')) {
            return (
                <span key={index} className="playbook-highlight-bracket">
                    {part}
                </span>
            );
        }
        return part;
    })
}