import 'element-plus/dist/index.css'
import { 
  ElForm, 
  ElFormItem, 
  ElButton, 
  ElInput, 
  ElInputNumber, 
  ElCheckbox, 
  ElSelect, 
  ElOption, 
  ElDialog, 
  ElTabs, 
  ElTabPane, 
  ElLoading,
  ElIcon,
  ElSwitch,
  ElDivider,
  ElMenu,
  ElMenuItem,
  ElSlider,
  ElDropdown,
  ElDropdownMenu,
  ElDropdownItem,
  ElContainer,
  ElFooter,
  ElTable,
  ElMessage 
} from 'element-plus'

export default (app: any) => {
  app.use(ElForm)
  app.use(ElFormItem)
  app.use(ElButton)
  app.use(ElInput)
  app.use(ElInputNumber)
  app.use(ElCheckbox)
  app.use(ElSelect)
  app.use(ElOption)
  app.use(ElDialog)
  app.use(ElTabs)
  app.use(ElTabPane)
  app.use(ElLoading)
  app.use(ElIcon)
  app.use(ElSwitch)
  app.use(ElDivider)
  app.use(ElMenu)
  app.use(ElMenuItem)
  app.use(ElSlider)
  app.use(ElDropdown)
  app.use(ElDropdownMenu)
  app.use(ElDropdownItem)
  app.use(ElContainer)
  app.use(ElFooter)
  app.use(ElTable)
  app.use(ElMessage)
}
