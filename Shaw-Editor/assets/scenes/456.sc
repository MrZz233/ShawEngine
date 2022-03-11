Scene: Untitled
Entities:
  - Entity: 12837192831273
    TagComponent:
      Tag: Green Square
    TransformComponent:
      Translation: [-0.300000012, 0, -2]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0, 1, 0, 1]
  - Entity: 12837192831273
    TagComponent:
      Tag: Red Square
    TransformComponent:
      Translation: [0.200000003, 0, 2]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 0, 0, 1]
  - Entity: 12837192831273
    TagComponent:
      Tag: Plane
    TransformComponent:
      Translation: [0, -0.5, 0]
      Rotation: [90, 0, 0]
      Scale: [10, 10, 10]
    SpriteRendererComponent:
      Color: [0.800000012, 0.800000012, 0.800000012, 1]
  - Entity: 12837192831273
    TagComponent:
      Tag: Camera A
    TransformComponent:
      Translation: [1, 0.800000012, 3.5]
      Rotation: [-25.5, 16.1000004, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 0
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 1
        PerspectiveFar: 1000
        OrthographicSize: 5
        OrthographicNear: -100
        OrthographicFar: 100
      Primary: true
      FixedAspectRatio: false
  - Entity: 12837192831273
    TagComponent:
      Tag: Camera B
    TransformComponent:
      Translation: [0, 0, 5]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 0
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 5
        OrthographicNear: -100
        OrthographicFar: 100
      Primary: false
      FixedAspectRatio: false