import React from 'react';
import styled from 'styled-components';
import BtnPrimary from './BtnPrimary';
import BtnSecondary from './BtnSecondary';

const ModalOverlay = styled.div`
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background: rgba(0, 0, 0, 0.3);
  backdrop-filter: blur(8px);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
`;

const ModalContainer = styled.div`
  background: var(--blue0);
  border-radius: 14px;
  padding: 40px;
  max-width: 400px;
  width: 90%;
  text-align: center;
  color: white;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
`;

const Title = styled.h2`
  margin: 0 0 20px 0;
  font-size: 1.8rem;
  font-weight: bold;
  color: white;
`;

const Description = styled.p`
  margin: 0 0 30px 0;
  font-size: 1rem;
  color: var(--gray3);
  line-height: 1.5;
`;

const Divider = styled.div`
  display: flex;
  align-items: center;
  margin: 20px 0;
  color: var(--gray3);
  font-size: 0.9rem;
  
  &::before,
  &::after {
    content: '';
    flex: 1;
    height: 1px;
    background: var(--gray4);
  }
  
  span {
    padding: 0 15px;
  }
`;

const Footer = styled.div`
  cursor: pointer;
  margin-top: 50px;
  padding: 20px;
  background: rgba(255, 255, 255, 0.1);
  border-radius: 8px;
  color: white;
  font-size: 1.1rem;
  font-weight: 600;
  text-align: center;
  text-shadow: 0 1px 2px rgba(0, 0, 0, 0.3);
`;

const BlackholeModal = ({ onInstallationGuide, onGetBlackhole, onLaunchSayso }) => {
  return (
    <ModalOverlay>
      <ModalContainer>
        <Title>One more step!</Title>
        <Description>
          Sayso needs Blackhole installed to work.
        </Description>
        
        <BtnPrimary 
          text="See our Installation Guide"
          onClick={onInstallationGuide}
        />
        
        <Divider>
          <span>or</span>
        </Divider>
        
        <BtnSecondary 
          text="Get Blackhole"
          onClick={onGetBlackhole}
          style={{color: 'white'}}
        />
        
        <Footer>
          Are you ready? <span onClick={onLaunchSayso}>Launch Sayso!</span>
        </Footer>
      </ModalContainer>
    </ModalOverlay>
  );
};

export default BlackholeModal;
