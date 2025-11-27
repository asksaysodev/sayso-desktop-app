const MIN_DURATION = 6000;  // 6 seconds minimum
const MAX_DURATION = 10000; // 10 seconds maximum (prevent annoyance)
const MS_PER_WORD = 400;    // 400ms per word (aprox 2.5 words/sec)

// Why 400ms per word?
// Slower than normal reading (250ms/word) to account for:
// Interruption/context-switching
// Comprehension time
// User might be doing something else
// Better to err on the side of "too long" than "too short"

/**
 * Calculate the display duration of an insight based on the text length.
 * @param {string} text - The text of the insight.
 * @returns {number} - The display duration in milliseconds.
 */
export default function calculateInsightDisplayDuration(text) {
    const buffer = text.trim().split(/\s+/);
    const bufferCount = buffer.length;
    const duration = bufferCount * MS_PER_WORD;
    const clampedDuration = Math.max(MIN_DURATION, Math.min(duration, MAX_DURATION));
    return clampedDuration; 
}