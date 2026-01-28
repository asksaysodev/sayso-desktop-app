import { AudioQueueItem } from "./globals";
import EventEmitter from 'events';

class AudioQueue extends EventEmitter {
  private queue: AudioQueueItem[];
  private isProcessing: boolean;
  private maxRetries: number;
  private retryDelay: number;
  private lastProcessedTimestamps: { [key: string]: number };

  constructor() {
    super();
    this.queue = [];
    this.isProcessing = false;
    this.maxRetries = 3;
    this.retryDelay = 2000; // 2 seconds
    this.lastProcessedTimestamps = {
      user: 0,
      prospect: 0
    };
  }

  // Add a new audio chunk to the queue
  enqueue(audioChunk: AudioQueueItem) {
    const queueItem = {
      ...audioChunk,
      retries: 0,
      status: 'pending',
      timestamp: Date.now()
    };
    
    // Insert into queue maintaining timestamp order for each speaker
    const insertIndex = this.queue.findIndex((item: AudioQueueItem) => 
      item.speaker === queueItem.speaker && 
      item.timestamp > queueItem.timestamp
    );
    
    if (insertIndex === -1) {
      this.queue.push(queueItem);
    } else {
      this.queue.splice(insertIndex, 0, queueItem);
    }
    
    console.log(`[Audio Queue] Added to queue: ${queueItem.filePath} (${queueItem.speaker}) - Queue length: ${this.queue.length}`);
    this.emit('queued', queueItem);
    
    // Start processing if not already processing
    if (!this.isProcessing) {
      this.processQueue();
    }
  }

  // Process the queue
  async processQueue() {
    if (this.isProcessing || this.queue.length === 0) {
      console.log(`[Audio Queue] Skipping process - isProcessing: ${this.isProcessing}, queueLength: ${this.queue.length}`);
      return;
    }

    this.isProcessing = true;
    console.log(`[Audio Queue] Starting queue processing - Current queue:`, this.queue.map((item: AudioQueueItem) => ({
      filePath: item.filePath,
      speaker: item.speaker,
      timestamp: item.timestamp,
      status: item.status
    })));
    
    while (this.queue.length > 0) {
      const currentItem = this.queue[0];
      console.log(`[Audio Queue] Current item:`, {
        filePath: currentItem.filePath,
        speaker: currentItem.speaker,
        timestamp: currentItem.timestamp,
        lastProcessedTimestamp: this.lastProcessedTimestamps[currentItem.speaker]
      });
      
      // Ensure we process items in timestamp order for each speaker
      if (currentItem.timestamp < this.lastProcessedTimestamps[currentItem.speaker]) {
        console.log(`[Audio Queue] Skipping out-of-order item: ${currentItem.filePath} (${currentItem.speaker}) - Current timestamp: ${currentItem.timestamp}, Last processed: ${this.lastProcessedTimestamps[currentItem.speaker]}`);
        this.queue.shift();
        continue;
      }
      
      try {
        currentItem.status = 'processing';
        console.log(`[Audio Queue] Processing item: ${currentItem.filePath} (${currentItem.speaker}) - Queue length: ${this.queue.length}`);
        this.emit('processing', currentItem);
        
        // Process the audio chunk
        console.log(`[Audio Queue] Calling onChunk for: ${currentItem.filePath}`);
        await currentItem.onChunk(currentItem.filePath, currentItem.speaker);
        
        // Success - remove from queue
        this.queue.shift();
        currentItem.status = 'completed';
        this.lastProcessedTimestamps[currentItem.speaker] = currentItem.timestamp;
        console.log(`[Audio Queue] Completed item: ${currentItem.filePath} (${currentItem.speaker}) - Remaining queue: ${this.queue.length}`);
        this.emit('completed', currentItem);
        
      } catch (error: any) {
        console.error(`[Audio Queue] Error processing audio chunk: ${error.message}`, error.stack);
        
        if (currentItem.retries < this.maxRetries) {
          // Retry with exponential backoff
          currentItem.retries++;
          currentItem.status = 'retrying';
          console.log(`[Audio Queue] Retrying item: ${currentItem.filePath} (${currentItem.speaker}) - Attempt ${currentItem.retries}`);
          this.emit('retrying', currentItem);
          
          const delay = this.retryDelay * Math.pow(2, currentItem.retries - 1);
          console.log(`[Audio Queue] Waiting ${delay}ms before retry`);
          await new Promise(resolve => setTimeout(resolve, delay));
          
          // Move to end of queue for retry
          this.queue.push(this.queue.shift() as AudioQueueItem);
          console.log(`[Audio Queue] Moved item to end of queue for retry`);
        } else {
          // Max retries reached - remove from queue
          this.queue.shift();
          currentItem.status = 'failed';
          console.error(`[Audio Queue] Failed item after ${currentItem.retries} retries: ${currentItem.filePath} (${currentItem.speaker})`);
          this.emit('failed', currentItem);
        }
      }
    }
    
    this.isProcessing = false;
    console.log('[Audio Queue] Queue processing completed - Queue is empty');
    this.emit('queueEmpty');
  }

  // Get current queue status
  getStatus() {
    return {
      queueLength: this.queue.length,
      isProcessing: this.isProcessing,
      items: this.queue.map((item: AudioQueueItem) => ({
        filePath: item.filePath,
        speaker: item.speaker,
        status: item.status,
        retries: item.retries,
        timestamp: item.timestamp
      }))
    };
  }

  // Clear the queue
  clear() {
    console.log('[Audio Queue] Clearing queue - Current length:', this.queue.length);
    this.queue = [];
    this.isProcessing = false;
    this.lastProcessedTimestamps = {
      user: 0,
      prospect: 0
    };
    this.emit('cleared');
  }
}

module.exports = new AudioQueue(); 