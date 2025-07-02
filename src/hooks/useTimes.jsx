export const useTimes = () => {

    const formatTime = (time) => {
    const date = new Date(time);
    const options = { 
        weekday: 'long',
        year: 'numeric', 
        month: 'long', 
        day: 'numeric',
        hour: '2-digit',
        minute: '2-digit',
        timeZoneName: 'short'
    };
    const formattedTime = date.toLocaleString('en-US', options);
    return formattedTime;
    }

    return {
        formatTime
    }
}