Module VB6toNET


    Public Function GetAppPath() As String
        Return System.IO.Path.GetDirectoryName( _
            System.Reflection.Assembly.GetExecutingAssembly().Location)
    End Function

End Module