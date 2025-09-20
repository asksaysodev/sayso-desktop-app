export default function CoachInsightContainer({displayInsight}) {

    return (
        <>
            {
                displayInsight && (
                    <div className={`coach-window-insights-container coach-box-bubble`}>
                        <p>{displayInsight}</p>
                    </div>
                )
            }
        </>
    )
}