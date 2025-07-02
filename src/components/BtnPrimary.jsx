import React from 'react';
import styled from 'styled-components';

const StyledButton = styled.button`
  background: ${({ $isGreen, $isCallActive }) => $isCallActive ? 'var(--red1)' : $isGreen ? 'var(--green1)' : 'var(--blue1)'};
  color: white;
  cursor: ${({ $isLoading }) => $isLoading ? 'not-allowed' : 'pointer'};
  border: ${({ $isGreen, $isCallActive }) => $isCallActive ? '1px solid var(--red1)' : $isGreen ? '1px solid var(--green1)' : '1px solid var(--blue1)'};
  border-radius: 6px;
  font-weight: bold;
  text-align: center;
  transition: all 0.2s ease-in-out;
  width: 100%;
  height: 40px;
  opacity: ${({ $isLoading }) => ($isLoading ? '0.7' : '1')};
  position: relative;
  z-index: 1;
  font-size: 0.95rem;

  &:hover {
    opacity: 0.95;
  }

  &:active {
    transform: scale(0.95);
  }
`;

const BtnPrimary = ({ text, onClick, type = 'button', isDisabled = false, isLoading = false, isGreen = false, isCallActive = false }) => {
  return (
    <StyledButton 
      onClick={onClick} 
      type={type} 
      disabled={isLoading} 
      $isLoading={isLoading}
      $isGreen={isGreen}
      $isCallActive={isCallActive}
    >
      {isLoading ? "Loading..." : text}
    </StyledButton>
  );
};

export default BtnPrimary; 