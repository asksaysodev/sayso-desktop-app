import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import { resolve } from 'path';
import { copyFileSync, existsSync } from 'fs';

// Custom plugin to copy .env files during build
const copyEnvFiles = () => {
  return {
    name: 'copy-env-files',
    writeBundle() {
      const envFiles = ['.env', '.env.production', '.env.development'];
      const destDirs = ['dist', 'electron']; // Copy to both directories
      
      destDirs.forEach(destDir => {
        envFiles.forEach(envFile => {
          const sourcePath = resolve(__dirname, envFile);
          const destPath = resolve(__dirname, destDir, envFile);
          
          if (existsSync(sourcePath)) {
            copyFileSync(sourcePath, destPath);
            console.log(`📁 Copied ${envFile} to ${destDir}/`);
          } else {
            console.log(`⚠️  ${envFile} not found, skipping...`);
          }
        });
      });
    }
  };
};

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    react(),
    copyEnvFiles() // Add our custom plugin
  ],
  optimizeDeps: {
    include: ['ws'],
  },
  base: './', // Use relative paths for Electron
  build: {
    outDir: 'dist', // Output directory for the build
    assetsDir: 'assets', // Assets directory
    rollupOptions: {
      input: {
        main: resolve(__dirname, 'index.html'),
        'coach-window': resolve(__dirname, 'coach-window.html'),
        'tray-menu': resolve(__dirname, 'tray-menu.html')
      },
      output: {
        manualChunks: undefined, // Disable manual chunks for better Electron compatibility
      },
    },
  },
  server: {
    port: 5173, 
    headers: {
      'Cross-Origin-Opener-Policy': 'same-origin',
      'Cross-Origin-Embedder-Policy': 'require-corp',
      'Cross-Origin-Resource-Policy': 'cross-origin',
      'Access-Control-Allow-Origin': '*',
      'Access-Control-Allow-Methods': 'GET, POST, PUT, DELETE, PATCH, OPTIONS',
      'Access-Control-Allow-Headers': 'X-Requested-With, content-type, Authorization'
    },
    proxy: {
      '/av': {
        target: 'https://source.zoom.us',
        changeOrigin: true,
        secure: false,
        headers: {
          'Access-Control-Allow-Origin': '*'
        }
      },
      '/lib': {
        target: 'https://source.zoom.us',
        changeOrigin: true,
        secure: false,
        headers: {
          'Access-Control-Allow-Origin': '*'
        }
      },
      '/ui': {
        target: 'https://source.zoom.us',
        changeOrigin: true,
        secure: false,
        headers: {
          'Access-Control-Allow-Origin': '*'
        }
      }
    }
  },
  // Ensure assets are properly handled
  publicDir: 'public',
  assetsInclude: ['**/*.png', '**/*.jpg', '**/*.jpeg', '**/*.gif', '**/*.svg', '**/*.ico', '**/*.icns'],
}); 