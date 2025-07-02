export default function CallCheckList({containerRef, isDragging, style, handleMouseDown, isVisible, hasEmergenceAnimationCompleted, signals}) {

    return (
        <div
          ref={containerRef}
          className={`floating-checklist-container ${isDragging ? 'dragging' : ''} ${isVisible && !hasEmergenceAnimationCompleted ? 'emerge-animation' : ''}`}
          style={style}
          onMouseDown={handleMouseDown}
        >
          <div className="drag-handle">
            <h1>Items to check</h1>
          </div>
            <div className="checklist-content">
              <ul>
                  <li className={signals?.pain_points ? 'checked' : ''}>
                  <span></span>
                  <p>Pain Points</p>
                  </li>
                  <li className={signals?.economic_impact ? 'checked' : ''}>
                  <span></span>
                  <p>Economic Impact</p>
                  </li>
                  <li className={signals?.decision_makers ? 'checked' : ''}>
                  <span></span>
                  <p>Decision Makers</p>
                  </li>
                  <li className={signals?.objections ? 'checked' : ''}>
                  <span></span>
                  <p>Buying Process Objections</p>
                  </li>
              </ul>
            </div>
        </div>
      );
}