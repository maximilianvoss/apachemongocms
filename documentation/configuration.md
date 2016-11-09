# Module Configuration
Values which can be set via Apache configuration while runtime.
  
## Database
To set up the database following schema can be used:


**Documents**

| Key                  | Type   | Example                    | Description                                             |   
| -------------------- | ------ | -------------------------- | ------------------------------------------------------- |
| DocMongoURI          | String | mongodb://localhost:27017/ | Setting the URI for the document's MongoDB              |
| DocMongoDB           | String | mongocms                   | DB in which the document's data shall be stored         |
| DocMongoCollection   | String | documents                  | Collection in which the document's data shall be stored |


**Assets**

| Key                  | Type   | Example                    | Description                                          |   
| -------------------- | ------ | -------------------------- | ---------------------------------------------------- |
| AssetMongoURI        | String | mongodb://localhost:27017/ | Setting the URI for the asset's MongoDB              |
| AssetMongoDB         | String | mongocms                   | DB in which the asset's data shall be stored         |
| AssetMongoCollection | String | assets                     | Collection in which the asset's data shall be stored |


**User**

| Key                  | Type   | Example                    | Description                                          |
| -------------------- | ------ | -------------------------- | ---------------------------------------------------- |
| UserMongoURI         | String | mongodb://localhost:27107/ | Setting the URI for the user's MongoDB               |
| UserMongoDB          | String | mongocms                   | DB in which the users's data shall be stored         |
| UserMongoCollection  | String | user                       | Collection in which the asset's data shall be stored |


## Input/Output parameter filter and mapper

**Documents**  

| Key                         | Type              | Example                | Description                                            |   
| --------------------------- | ----------------- | ---------------------- | ------------------------------------------------------ |
| DocumentPropInputWhitelist  | String[]          | parameter1             | White list of parameters which will be accepted        |
| DocumentPropOutputWhitelist | String[]          | parameter2             | White list of parameters which will be in the response |
| DocumentPropMapping         | (String String)[] | parent#child mappedKey | Transforms the database keys to a mapped key           |


**Assets**

| Key                               | Type              | Example                | Description                                            |   
| --------------------------------- | ----------------- | ---------------------- | ------------------------------------------------------ |
| AssetMetadataParamInputWhitelist  | String            | parameter1             | White list of parameters which will be accepted        |
| AssetMetadataParamOutputWhitelist | String            | parameter2             | White list of parameters which will be in the response |
| AssetMetadataParamMapping         | (String String)[] | parent#child mappedKey | Transforms the database keys to a mapped key           |


**User**

| Key                             | Type              | Example                | Description                                            |
| ------------------------------- | ----------------- | ---------------------- | ------------------------------------------------------ |
| UserProfileParamInputWhitelist  | String[]          | parameter1             | White list of parameters which will be accepted        |
| UserProfileParamOutputWhitelist | String[]          | parameter2             | White list of parameters which will be in the response |
| UserProfileParamMapping         | (String String)[] | parent#child mappedKey | Transforms the database keys to a mapped key           |


##Query Lists

| Key               | Type              | Example                        | Description                                        |
| ----------------- | ----------------- | ------------------------------ | -------------------------------------------------- |
| DocumentQueryList | (String String)[] | all { }                        | Searches in Document collection if list.all.json   |
| AssetQueryList    | (String String)[] | specific {"name": "abc"}       | Searches in Asset collection if list.specific.json |
| UserQueryList     | (String String)[] | requested {"name": "@{param1}" | Searches in User collection if list.requested.json |

  
##Asset specifics

| Key            | Type           | Example                  | Description                                                    |   
| -------------- | -------------- | ------------------------ | -------------------------------------------------------------- |
| AssetStorePath | Path           | /var/www/html/assetstore | Path where the assets shall be stored                          |
| AssetTmpPath   | Path           | /tmp/asset               | Path and file-prefix to store assets temporary while uploading |
| AssetTransform | (String Int)[] | large 1000               | Setting names for image transformation widths                  |


##User specifics

| Key                       | Type | Example | Description                                       |
| ------------------------- | ---- | ------- | ------------------------------------------------- |
| UserSessionsMaxCount      | Int  | 10      | the maximum of sessions a user is allowed to open |
| UserSessionExpirationTime | Int  | 1       | amount of hours a session expires                 |