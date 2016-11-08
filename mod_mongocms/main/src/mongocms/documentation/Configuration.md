# Apache Configuration
Values which can be set via Apache Configuration while runtime.  

**Documents**  

| Key                | Type     | Example                    | Description                                             |   
| ------------------ | -------- | -------------------------- | ------------------------------------------------------- |
| DocMongoURI        | String   | mongodb://localhost:27017/ | Setting the URI for the document's MongoDB              |
| DocMongoDB         | String   | publisher                  | DB in which the document's data shall be stored         |
| DocMongoCollection | String   | documents                  | Collection in which the document's data shall be stored |
| propWhitelistIn          | String[] | parameter                  | a parameter for writing documents                       |
| HtmlEscape         | String[] | parameter                  | Escaping the HTML of this particular parameter          |
  
  
**Assets**  

| Key                               | Type              | Example                    | Description                                                    |   
| --------------------------------- | ----------------- | -------------------------- | -------------------------------------------------------------- |
| AssetMongoURI                     | String            | mongodb://localhost:27017/ | Setting the URI for the asset's MongoDB                        |
| AssetMongoDB                      | String            | publisher                  | DB in which the asset's data shall be stored                   |
| AssetMongoCollection              | String            | assets                     | Collection in which the asset's data shall be stored           |
| AssetStorePath                    | Path              | /var/www/html/assetstore   | Path where the assets shall be stored                          |
| AssetTmpPath                      | Path              | /tmp/asset                 | Path and file-prefix to store assets temporary while uploading |
| AssetTransform                    | (String Int)[]    | large 1000                 | Setting names for image transformation widths                  |
| AssetMetadataParamInputWhitelist  | String            | parameter                  | A white listed parameter to input asset properties             |
| AssetMetadataParamOutputWhitelist | String            | parameter                  | A white listed parameter to output asset properties            |
| AssetMetadataParamMapping         | (String String)[] | dbkey requestkey           | transforms the key dbkey to requestkey, vice versa             |


**User**

| Key                             | Type              | Example                    | Description                                          |
| ------------------------------- | ----------------- | -------------------------- | ---------------------------------------------------- |
| UserMongoURI                    | String            | mongodb://localhost:27107/ | Setting the URI for the user's MongoDB               |
| UserMongoDB                     | String            | publisher                  | DB in which the users's data sahll be stored         |
| UserMongoCollection             | String            | user                       | Collection in which the asset's data shall be stored |
| UserProfileParamMapping         | (String String)[] | dbkey request key          | transforms the dbkey to requestkey, vice versa       |
| UserProfileParamInputWhitelist  | String[]          | parameter                  | a parameter which is allowed to be written to db     |
| UserProfileParamOutputWhitelist | String[]          | parameter                  | a parameter which is allowed to be exposed from db   |
| UserSessionsMaxCount            | Int               | 10                         | the maximum of sessions a user is allowed to open    |
| UserSessionExpirationTime       | Int               | 1                          | amount of hours a session expires                    |