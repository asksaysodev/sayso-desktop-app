import { createContext, useContext, useEffect, useState } from 'react'
import { useAuth } from './AuthContext'
import { useProspects } from '../hooks/useProspects'

const ProspectsContext = createContext({})

export const ProspectsProvider = ({ children }) => {
  const { globalUser } = useAuth()
  const [prospects, setProspects] = useState([])
  const [loading, setLoading] = useState(true)

  const { getAccountProspects } = useProspects()

  const fetchProspects = async () => {
    if (globalUser) {
      setLoading(true)
      try {
        const prospectsData = await getAccountProspects()
        if(prospectsData && prospectsData.length > 0) {
          prospectsData.sort((a, b) => a.name .localeCompare(b.name))
          setProspects(prospectsData)
        }
        else {
          setProspects([])
        }
      } catch (error) {
        console.error('Error fetching prospects:', error)
      } finally {
        setLoading(false)
      }
    }
  }

  useEffect(() => {
    fetchProspects()
  }, [globalUser])

  const value = {
    prospects,
    setProspects,
    loading,
    fetchProspects
  }

  return (
    <ProspectsContext.Provider value={value}>
      {children}
    </ProspectsContext.Provider>
  )
}

export const useProspectsContext = () => {
  return useContext(ProspectsContext)
} 