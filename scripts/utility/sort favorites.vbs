const HKEY_CURRENT_USER = &H80000001

DeleteRegistryKey HKEY_CURRENT_USER, "Software\Microsoft\Windows\CurrentVersion\Explorer\MenuOrder\Favorites"

Sub DeleteRegistryKey(RegRoot, strPath)
   Dim strRegistryKeys, objRegistry, strKey

   Set objRegistry = GetObject("winmgmts:\\.\root\default:StdRegProv")
   objRegistry.EnumKey RegRoot, strPath, strRegistryKeys

   If IsArray(strRegistryKeys) Then
      For each strKey in strRegistryKeys
           DeleteRegistryKey RegRoot, strPath & "\" & strKey
      Next
   End If

   objRegistry.DeleteKey RegRoot, strPath
End Sub